/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:02:10 by leia              #+#    #+#             */
/*   Updated: 2025/10/24 02:02:39 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

int	execute(t_mini *cmd, t_env *env_list, char **envp)
{
	int		rc;
	t_shell	shell;

	if (is_empty_input(cmd))
		return (g_last_status);
	rc = prepare_all_heredocs(cmd);
	if (rc != 0)
		return (g_last_status);
	if (cmd->next == NULL && needs_parent_execution(cmd))
	{
		exec_single_builtin_parent(cmd, env_list);
		return (g_last_status);
	}
	init_shell(&shell, envp, env_list);
	rc = exec_pipeline(cmd,&shell);
	if (rc == 0)
		wait_for_all_children(shell.pids, shell.node_count);
	if (shell.pipes)
		free(shell.pipes);
	if (shell.pids)
		free(shell.pids);
	return (g_last_status);
}

static int is_empty_input(t_mini *mini_head)
{
    if (!mini_head)
        return (1);
    if (!mini_head->argv)
        return (1);
    if (!mini_head->argv[0] || mini_head->argv[0][0] == '\0')
        return (1);
    return (0);
}

static int needs_parent_execution(t_mini *cmd)
{
    const char *name;
    int i;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (0);
    name = cmd->argv[0];
    if (ft_strcmp(name, "cd") == 0)
        return (1);
    if (ft_strcmp(name, "unset") == 0)
        return (1);
    if (ft_strcmp(name, "exit") == 0)
        return (1);
    if (ft_strcmp(name, "export") == 0)
    {
        if (cmd->argv[1] == NULL)
            return (0); /* export (no args): prints env → child OK */
        i = 1;
        while (cmd->argv[i])
        {
            if (ft_strchr(cmd->argv[i], '=') != NULL)
                return (1); /* export VAR=... → parent */
            i++;
        }
        return (0);
    }
    return (0); /* echo/pwd/env/external → not parent-only */
}

static void	init_shell(t_shell *sh, t_env *env_list, char **envp)
{
	sh->pipes = NULL;
	sh->pids = NULL;
	sh->node_index = 0;
	sh->node_count = 0;
	sh->prev_read_end = -1;
	sh->env_list = env_list;
	sh->vars = NULL;
	sh->envp = envp;
}

int	wait_for_all_children(pid_t *pids, int n)
{
	int		waited;
	int		st;
	pid_t	pid;

	if (!pids || n <= 0)
		return (0);
	waited = 0;
	while (waited < n)
	{
		pid = waitpid(-1, &st, 0);
		if (pid <= 0)
			break ;
		if (pid == pids[n - 1])
		{
			if (WIFSIGNALED(st))
			{
				if (WTERMSIG(st) == SIGINT)
					write(1, "\n", 1);
				g_last_status = 128 + WTERMSIG(st);
			}
			else if (WIFEXITED(st))
				g_last_status = WEXITSTATUS(st);
			else
				g_last_status = 1;
		}
		waited++;
	}
	return (0);
}
