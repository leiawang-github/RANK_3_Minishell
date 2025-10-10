/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:03:01 by leia              #+#    #+#             */
/*   Updated: 2025/09/23 22:31:15 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

/* Static function declarations */
static int count_pipeline_nodes(t_mini *pipeline);
static void cleanup_pipes(int **pipes, int count);
static void cleanup_and_free_pipes(int **pipes, int count);
static int **create_pipes(int pipe_count);
static void close_pipes(int **pipes, int pipe_count);
static void connect_pipeline(t_shell *shell);
static void close_unused_pipes(t_shell *shell);
static int exec_builtin_in_pipeline(t_mini *cmd, t_env *env_list);
static int execute_pipeline_node(t_mini *cmd, t_shell *shell);
static int wait_for_all_children(pid_t *pids, int node_count);
static void kill_children(pid_t *pids, int count);

int exec_pipeline(t_mini *pipeline, char **envp, t_env *env_list)
{
    int node_count;
    int **pipes; // array of pipes
    pid_t *pids; // array of child PIDs
    t_mini *current;
    int i;
    
    // 设置信号处理，忽略 SIGINT 和 SIGQUIT 在父进程中
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    node_count = count_pipeline_nodes(pipeline); // 计算管道节点数，一定大于1
    pipes = create_pipes(node_count - 1); // 创建所需的管道array
    if (!pipes)
        return -1; // g_last_status 已在 create_pipes 中设置
    pids = malloc(sizeof(pid_t) * node_count); // 存储每个子进程的PID
    if (!pids)
    {
        close_pipes(pipes, node_count - 1);
        free(pipes);
        ft_errno("malloc", errno, ERR_SYS_BUILTIN);
        return -1;
    }
    current = pipeline;
    i = 0;
    while (current && i < node_count) // 对每个节点创建子进程
    {
        pids[i] = fork(); // 创建子进程
        if (pids[i] == 0) // 子进程：执行当前节点
        {
            // 子进程中恢复默认信号处理
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            t_shell shell = 
            {
                .pipes = pipes,
                .node_index = i,
                .node_count = node_count,
                .env_list = env_list,
                .envp = envp
            };
            execute_pipeline_node(current, &shell);
            exit(g_last_status); // 不应该到达这里，使用正确的退出码
        }
        else if (pids[i] < 0)
        {
            ft_errno("fork", errno, ERR_SYS_BUILTIN);
            // 杀死已经fork的子进程
            kill_children(pids, i);
            // 等待已fork的子进程
            wait_for_all_children(pids, i);
            close_pipes(pipes, node_count - 1);
            free(pipes);
            free(pids);
            return -1;
        }
        current = current->next;
        i++;
    }
    close_pipes(pipes, node_count - 1);
    free(pipes);
    wait_for_all_children(pids, node_count);
    free(pids);
    
    // 恢复REPL模式的信号处理
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);  // 与主循环保持一致
    
    return g_last_status;
}

static int count_pipeline_nodes(t_mini *pipeline)
{
    int count;
    t_mini *current;

    count = 0;
    current = pipeline;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

static int **create_pipes(int pipe_count)
{
    int **pipes;
    int i;

    i = 0;
    pipes = malloc(sizeof(int*) * pipe_count); //malloc array of int*[2]
    if (!pipes)
        return (ft_errno("malloc", errno, ERR_SYS_BUILTIN), NULL);
    while (i < pipe_count)
    {
        pipes[i] = malloc(sizeof(int) * 2); //malloc each int[2]
        if (!pipes[i])
        {
            cleanup_and_free_pipes(pipes, i);
            return (ft_errno("malloc", errno, ERR_SYS_BUILTIN), NULL);
        }
        if (pipe(pipes[i]) == -1)
        {
            cleanup_and_free_pipes(pipes, i);
            return (ft_errno("pipe", errno, ERR_SYS_BUILTIN), NULL);
        }
        i++;
    }
    return pipes;
}

static void close_pipes(int **pipes, int pipe_count)
{
    int i;

    i = 0;
    while (i < pipe_count)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
        i++;
    }
}

static void connect_pipeline(t_shell *shell)
{
    if (shell->node_index == 0) // 第一个节点
    {
        dup2(shell->pipes[0][1], STDOUT_FILENO);
        close(shell->pipes[0][0]);
        close(shell->pipes[0][1]);
    }
    else if (shell->node_index == shell->node_count - 1) // 最后一个节点
    {
        dup2(shell->pipes[shell->node_index - 1][0], STDIN_FILENO);
        close(shell->pipes[shell->node_index - 1][0]);
        close(shell->pipes[shell->node_index - 1][1]);
    }
    else // 中间节点
    {
        dup2(shell->pipes[shell->node_index - 1][0], STDIN_FILENO); 
        dup2(shell->pipes[shell->node_index][1], STDOUT_FILENO);
        close(shell->pipes[shell->node_index - 1][0]); 
        close(shell->pipes[shell->node_index - 1][1]); 
        close(shell->pipes[shell->node_index][0]); 
        close(shell->pipes[shell->node_index][1]);
    }
}

static void close_unused_pipes(t_shell *shell)
{
    int i;

    i = 0;
    while (i < shell->node_count - 1) // 关闭未使用的管道端口
    {
        // 关闭当前节点不需要的管道
        if (i != shell->node_index - 1 && i != shell->node_index)
        {
            close(shell->pipes[i][0]);
            close(shell->pipes[i][1]);
        }
        i++;
    }
}

static int execute_pipeline_node(t_mini *cmd, t_shell *shell)
{
    t_cmd_type cmd_type;
    
    connect_pipeline(shell);
    close_unused_pipes(shell);
    if (exec_redirs(cmd->redirs) != 0)
        exit(g_last_status);
    
    cmd_type = analyze_cmd(cmd);
    if (cmd_type == CMD_BUILTIN)
    {
        // Pipeline 中的内建命令：在子进程中执行，不影响父进程环境
        exit(exec_builtin_in_pipeline(cmd, shell->env_list));
    }
    else if (cmd_type == CMD_EXTERNAL)
    {
        // 外部命令执行
        char *path = which_path(cmd->cmd_argv[0], shell->env_list);
        if (!path)
        {
            err_msg(cmd->cmd_argv[0], ": command not found", ERR_CMD_NOT_FOUND);
            exit(127);
        }
        
        execve(path, cmd->cmd_argv, shell->envp);
        ft_errno(cmd->cmd_argv[0], errno, ERR_CANNOT_EXEC);
        free(path);
        exit(126);
    }
    else
    {
        // 其他类型（如无效命令）
        exit(127);
    }
}

static int wait_for_all_children(pid_t *pids, int node_count)
{
    int i;
    int status;

    i = 0;
    while (i < node_count)
    {
        while (waitpid(pids[i], &status, 0) == -1)
        {
            if (errno != EINTR)
                break;
        }
        
        // 记录最后一个命令的退出状态（bash 行为）
        if (i == node_count - 1)
        {
            if (WIFEXITED(status))
                g_last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                g_last_status = 128 + WTERMSIG(status);
        }
        
        i++;
    }
    
    return g_last_status;
}

static int exec_builtin_in_pipeline(t_mini *cmd, t_env *env_list)
{
    char *cmd_name;

    if (!cmd || !cmd->cmd_argv || !cmd->cmd_argv[0])
        return 1;
    
    cmd_name = cmd->cmd_argv[0];
    
    if (ft_strcmp(cmd_name, "echo") == 0)
        return builtin_echo(cmd->cmd_argv);
    else if (ft_strcmp(cmd_name, "cd") == 0)
    {
        // Pipeline 中的 cd：在子进程中执行，不影响父进程工作目录
        return builtin_cd(cmd->cmd_argv, env_list);
    }
    else if (ft_strcmp(cmd_name, "pwd") == 0)
        return builtin_pwd();
    else if (ft_strcmp(cmd_name, "export") == 0)
    {
        // Pipeline 中的 export：在子进程中执行，不影响父进程环境
        return builtin_export(cmd->cmd_argv, &env_list);
    }
    else if (ft_strcmp(cmd_name, "unset") == 0)
    {
        // Pipeline 中的 unset：在子进程中执行，不影响父进程环境
        return builtin_unset(cmd->cmd_argv, &env_list);
    }
    else if (ft_strcmp(cmd_name, "env") == 0)
        return builtin_env(cmd->cmd_argv, env_list);
    else if (ft_strcmp(cmd_name, "exit") == 0)
        return builtin_exit(cmd->cmd_argv);
    
    return 127; // Not a recognized builtin
}

static void cleanup_pipes(int **pipes, int count)
{
    while (count >= 0)
    {
        free(pipes[count]);
        count--;
    }
}

static void cleanup_and_free_pipes(int **pipes, int count)
{
    cleanup_pipes(pipes, count);
    free(pipes);
}

/* 发送SIGTERM到所有已fork的子进程 */
static void kill_children(pid_t *pids, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        if (pids[i] > 0)
            kill(pids[i], SIGTERM);
        i++;
    }
}