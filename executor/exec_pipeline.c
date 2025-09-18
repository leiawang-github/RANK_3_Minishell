/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:03:01 by leia              #+#    #+#             */
/*   Updated: 2025/09/17 23:13:20 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/env_copy.h"

/* Static function declarations */
static int count_pipeline_nodes(t_cmd *pipeline);
static int create_pipes(int **pipes, int pipe_count);
static void close_pipes(int **pipes, int pipe_count);
static void connect_pipeline_io(int **pipes, int node_index, int node_count);
static void close_unused_pipes(int **pipes, int node_index, int node_count);
static int exec_builtin_in_pipeline(t_cmd *cmd, t_env *env_list);
static int execute_pipeline_node(t_cmd *cmd, int **pipes, int node_index, 
                                int node_count, t_env *env_list, char **envp);
static int wait_for_all_children(pid_t *pids, int node_count);

int exec_pipeline(t_cmd *pipeline, char **envp, t_env *env_list)
{
    int node_count;
    int **pipes; // array of pipes
    pid_t *pids; // array of child PIDs
    t_cmd *current;
    int i;

    if (!pipeline)
        return -1; // 防御性检查
    node_count = count_pipeline_nodes(pipeline); // 计算管道节点数，一定大于1
    pipes = malloc(sizeof(int*) * (node_count - 1));
    if (!pipes)
        return -1;
    if (create_pipes(pipes, node_count - 1) != 0)
    {
        free(pipes);
        return -1;
    }
    pids = malloc(sizeof(pid_t) * node_count); // 存储每个子进程的PID
    if (!pids)
    {
        close_pipes(pipes, node_count - 1);
        free(pipes);
        return -1;
    }
    current = pipeline;
    i = 0;
    while (current && i < node_count) // 对每个节点创建子进程
    {
        pids[i] = fork(); // 创建子进程
        if (pids[i] == 0) // 子进程：执行当前节点
        {
            execute_pipeline_node(current, pipes, i, node_count, env_list, envp);
            exit(1); // 不应该到达这里
        }
        else if (pids[i] < 0)
        {
            ft_errno("fork", errno, ERR_SYS_BUILTIN);
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
    return g_last_status;
}

static int count_pipeline_nodes(t_cmd *pipeline)
{
    int count;
    t_cmd *current;

    count = 0;
    current = pipeline;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

static int create_pipes(int **pipes, int pipe_count)
{
    int i;

    i = 0;
    while (i < pipe_count)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i])
        {
            while (i > 0)
            {
                i--;
                free(pipes[i]);
            }
            return -1;
        }
        if (pipe(pipes[i]) == -1)
        {
            ft_errno("pipe", errno, ERR_SYS_BUILTIN);
            while (i >= 0)
            {
                free(pipes[i]);
                i--;
            }
            return -1;
        }
        i++;
    }
    return 0;
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

static void connect_pipeline_io(int **pipes, int node_index, int node_count)
{
    if (node_index == 0) // 第一个节点
    {
        dup2(pipes[0][1], STDOUT_FILENO);
        close(pipes[0][0]);
        close(pipes[0][1]);
    }
    else if (node_index == node_count - 1) // 最后一个节点
    {
        dup2(pipes[node_index - 1][0], STDIN_FILENO);
        close(pipes[node_index - 1][0]);
        close(pipes[node_index - 1][1]);
    }
    else // 中间节点
    {
        dup2(pipes[node_index - 1][0], STDIN_FILENO); 
        dup2(pipes[node_index][1], STDOUT_FILENO);
        close(pipes[node_index - 1][0]); 
        close(pipes[node_index - 1][1]); 
        close(pipes[node_index][0]); 
        close(pipes[node_index][1]);
    }
}

static void close_unused_pipes(int **pipes, int node_index, int node_count)
{
    int i;

    i = 0;
    while (i < node_count - 1) // 关闭未使用的管道端口
    {
        if (i != node_index - 1 && i != node_index)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        i++;
    }
}

static int execute_pipeline_node(t_cmd *cmd, int **pipes, int node_index, 
                                int node_count, t_env *env_list, char **envp)
{
    t_cmd_type cmd_type;
    connect_pipeline_io(pipes, node_index, node_count);
    close_unused_pipes(pipes, node_index, node_count);
    if (exec_redirs(cmd->redirs) != 0)
        exit(1);
    cmd_type = analyze_cmd(cmd);
    if (cmd_type == CMD_BUILTIN)
    {
        // Pipeline 中的内建命令：在子进程中执行，不影响父进程环境
        exit(exec_builtin_in_pipeline(cmd, env_list));
    }
    else if (cmd_type == CMD_EXTERNAL)
    {
        // 外部命令执行
        char *path = which_path(cmd->argv[0], env_list);
        if (!path)
        {
            err_msg(cmd->argv[0], ": command not found", ERR_CMD_NOT_FOUND);
            exit(127);
        }
        
        execve(path, cmd->argv, envp);
        ft_errno(cmd->argv[0], errno, ERR_CANNOT_EXEC);
        free(path);
        exit(126);
    }
    else
    {
        // 其他类型（如重定向错误）
        exit(1);
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
            g_last_status = WEXITSTATUS(status);
        
        i++;
    }
    
    return g_last_status;
}

static int exec_builtin_in_pipeline(t_cmd *cmd, t_env *env_list)
{
    char *cmd_name;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return 1;
    
    cmd_name = cmd->argv[0];
    
    if (ft_strcmp(cmd_name, "echo") == 0)
        return builtin_echo(cmd->argv);
    else if (ft_strcmp(cmd_name, "cd") == 0)
        return builtin_cd(cmd->argv, env_list);
    else if (ft_strcmp(cmd_name, "pwd") == 0)
        return builtin_pwd();
    else if (ft_strcmp(cmd_name, "export") == 0)
        return builtin_export(cmd->argv, &env_list);
    else if (ft_strcmp(cmd_name, "unset") == 0)
        return builtin_unset(cmd->argv, &env_list);
    else if (ft_strcmp(cmd_name, "env") == 0)
        return builtin_env(cmd->argv, env_list);
    else if (ft_strcmp(cmd_name, "exit") == 0)
        return builtin_exit(cmd->argv);
    
    return 127; // Not a recognized builtin
}