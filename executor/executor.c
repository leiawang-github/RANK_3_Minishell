/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:02:10 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 15:45:24 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

int execute(t_mini *cmd, t_env *env_list, char **envp)
{
    int rc;

    if (is_empty_input(cmd))
        return (g_last_status);
    rc = prepare_all_heredocs(cmd);
    if (rc != 0)
        return (g_last_status);
    if (cmd->next == NULL && needs_parent_execution(cmd))
    {
        rc = exec_single_builtin_parent(cmd, env_list);
        restore_parent_fds_after_builtin();
        (void)rc;
        return (g_last_status);
    }
    exec_pipeline(cmd, envp, env_list);
    wait_for_all_children();
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


