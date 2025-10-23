/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:30:30 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 13:48:59 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

static int is_builtin(t_mini *mini_head)
{
    const char *cmd_name;

    cmd_name = mini_head->argv[0];

    return (ft_strcmp(cmd_name, "echo") == 0 ||
            ft_strcmp(cmd_name, "cd") == 0 ||
            ft_strcmp(cmd_name, "pwd") == 0 ||
            ft_strcmp(cmd_name, "env") == 0 ||
            ft_strcmp(cmd_name, "unset") == 0 ||
            ft_strcmp(cmd_name, "exit") == 0 ||
            ft_strcmp(cmd_name, "export") == 0);
}

t_cmd_type analyze_cmd(t_mini *mini_head)
{
    if(!mini_head)
        return CMD_INVALID;
    if (!mini_head->argv || !mini_head->argv[0])
    {
        if(mini_head->redirs)
            return CMD_REDIR_ONLY;
        return CMD_INVALID;
    }
    if (is_builtin(mini_head))
        return CMD_BUILTIN;
    return CMD_EXTERNAL;
}

