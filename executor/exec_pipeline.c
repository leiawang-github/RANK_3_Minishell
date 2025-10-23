/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 23:03:01 by leia              #+#    #+#             */
/*   Updated: 2025/10/24 00:24:20 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

/*
[exec_pipeline(head, envp, env_list)]
        |
        v
[统计节点数 N; 若 N<=0 -> return 0]
        |
        v
[为 N-1 条边创建 pipes(数组)]
        |
        v
[i = 0; prev_read_fd = -1]
        |
        v
[循环 while i < N 逐节点启动]
        |
        v
[若不是最后一个节点 -> 建立 pipe curr_pipe]
        |
        v
[fork() 子进程?] --error--> [打印错误; 关闭所有pipe; g_last_status=1; return 1]
        |
        +-- child -------------------------------+
        |                                        |
        |   [恢复默认信号: SIGINT/SIGQUIT]       |
        |   [接线stdin:                           |
        |     i==0 ? 用父STDIN : dup2(prev_read_fd, STDIN)]
        |   [接线stdout:                          |
        |     i==N-1 ? 用父STDOUT : dup2(curr_pipe.write, STDOUT)]
        |   [关闭所有无关pipe端与 prev_read_fd]  |
        |   [应用本节点重定向 (incl. heredoc_fd->STDIN)]
        |        --error--> [_exit(1)]           |
        |   [是否 builtin(子进程可执行型)?]       |
        |        yes --> [run_builtin_child(argv, env_list); _exit(ret)]
        |        no  --> [解析可执行路径:         |
        |                    argv[0] 含'/'? 直接尝试 |
        |                    否则遍历 PATH 拼接   ]
        |              [execve(path, argv, envp)]|
        |                 --成功不返回--         |
        |                 --失败-->              |
        |                 [errno == ENOENT ?     |
        |                      _exit(127) : _exit(126)]
        |
        +-- parent ------------------------------+
                 |
                 v
        [记录 pid[i]]
                 |
                 v
        [关闭 curr_pipe.write (若存在)]
                 |
                 v
        [若 prev_read_fd != -1 -> 关闭它]
                 |
                 v
        [prev_read_fd = (i==N-1 ? -1 : curr_pipe.read)]
                 |
                 v
        [i++ 循环继续]
        |
        v
[循环结束: 若 prev_read_fd != -1 关闭它; 关闭剩余管道端]
        |
        v
[return 0]   // 等待与最终状态由外层 wait_for_all_children() 完成
*/

int exec_pipeline(t_mini *cmd, t_shell *sh)
{
    t_mini *node;

	sh->node_count = count_pipeline_nodes(cmd);
	if (sh->node_count <= 0)
		return (0);
	if (setup_pids_and_pipes(sh) != 0)
		return (g_last_status = 1);
    sh->node_index = 0;
	sh->prev_read_end = -1;
	node = cmd;
	if (exec_all_children(node, sh) != 0)
	{
		free_pids_and_pipes(sh);
		return (1);
	}
	return (0);
}

static int	count_pipeline_nodes(t_mini *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static int	setup_pids_and_pipes(t_shell *sh)
{
	if (sh->node_count > 0)
	{
		sh->pids = malloc(sizeof(pid_t) * sh->node_count);
		if (!sh->pids)
			return (-1);
	}
	if (sh->node_count > 1)
	{
		sh->pipes = malloc(sizeof(*sh->pipes) * (sh->node_count - 1));
		if (!sh->pipes)
		{
			free(sh->pids);
			sh->pids = NULL;
			return (-1);
		}
		if (setup_pipes(sh->pipes, sh->node_count - 1) != 0)
		{
			free(sh->pipes);
			sh->pipes = NULL;
			free(sh->pids);
			sh->pids = NULL;
			return (-1);
		}
	}
	return (0);
}

static void free_pids_and_pipes(t_shell *sh)
{
	if (sh->pipes)
		free(sh->pipes);
	if (sh->pids)
		free(sh->pids);
    sh->pipes = NULL;
	sh->pids = NULL;
}

static void	cleanup_pipes(int pipes[][2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
