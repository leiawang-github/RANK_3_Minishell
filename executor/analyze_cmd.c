/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:30:30 by leia              #+#    #+#             */
/*   Updated: 2025/09/16 22:54:40 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

static int is_builtin(t_mini *pipeline)
{
    const char *cmd_name;
    
    cmd_name = pipeline->cmd_argv[0];
    
    return (ft_strcmp(cmd_name, "echo") == 0 ||
            ft_strcmp(cmd_name, "cd") == 0 ||
            ft_strcmp(cmd_name, "pwd") == 0 ||
            ft_strcmp(cmd_name, "env") == 0 ||
            ft_strcmp(cmd_name, "unset") == 0 ||
            ft_strcmp(cmd_name, "exit") == 0 ||
            ft_strcmp(cmd_name, "export") == 0);
}

t_cmd_type analyze_cmd(t_mini *pipeline) 
{
    if(!pipeline)
        return CMD_INVALID;
    if (!pipeline->cmd_argv || !pipeline->cmd_argv[0]) 
    {
        if(pipeline->redirs)
            return CMD_REDIR_ONLY; 
        return CMD_INVALID;
    }
    if (is_builtin(pipeline))
        return CMD_BUILTIN;
    return CMD_EXTERNAL;
}

