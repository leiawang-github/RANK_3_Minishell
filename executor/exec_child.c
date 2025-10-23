/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 23:11:22 by leiwang           #+#    #+#             */
/*   Updated: 2025/10/24 01:50:15 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

int	exec_all_children(t_mini *node, t_shell *sh)
{
	int	i;
	int	prev;

	i = 0;
	prev = -1;
	while (node)
	{
		sh->node_index = i;
		sh->prev_read_end = prev;
		if (exec_one_child(node, sh) != 0)
		{
			if (sh->pipes)
				cleanup_pipes(sh->pipes, sh->node_count - 1);
			return (-1);
		}
		if (i > 0 && prev != -1)
			close(prev);
		if (sh->pipes && i < sh->node_count - 1)
			prev = sh->pipes[i][0];
		if (sh->pipes)
			close(sh->pipes[i][1]);
		node = node->next;
		i++;
	}
	if (sh->pipes)
		cleanup_pipes(sh->pipes, sh->node_count - 1);
	return (0);
}

int	exec_one_child(t_mini *cmd, t_shell *sh)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		g_last_status = 1;
		return (-1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		connect_pipes_for_child(sh);
		close_all_pipes_child(sh);
		if (apply_all_redirs(cmd->redirs) != 0)
			_exit(1);
		if (is_child_builtin(cmd))
			_exit(run_builtin_child(cmd, sh->env_list));
		run_external(cmd->argv, sh->envp);
		_exit(exec_error_status(errno));
	}
	sh->pids[sh->node_index] = pid; //父进程：记录 pid；不关闭任何 pipe（交给调用者）
	return (0);
}

static void	connect_pipes_for_child(t_shell *sh)
{
	int	i;

	i = sh->node_index;
	if (i > 0 && sh->prev_read_end != -1)
	{
		if (dup2(sh->prev_read_end, STDIN_FILENO) < 0)
			_exit(1);
	}
	if (sh->pipes && i < sh->node_count - 1)
	{
		if (dup2(sh->pipes[i][1], STDOUT_FILENO) < 0)
			_exit(1);
	}
}

static int	exec_error_status(int err)
{
	if (err == ENOENT)
		return (ERR_CMD_NOT_FOUND);
	if (err == EACCES || err == EISDIR)
		return (ERR_CANNOT_EXEC);
}

