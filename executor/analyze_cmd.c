/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:30:30 by leia              #+#    #+#             */
/*   Updated: 2025/09/15 20:14:14 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

t_cmd_type analyze_cmd(t_cmd *pipeline) 
{
    if(!pipeline)
        return CMD_INVALID;
    if (!pipeline->argv || !pipeline->argv[0]) 
    {
        if(pipeline->redirs)
            return CMD_REDIR_ONLY; 
        return CMD_INVALID;
    }
    if (is_builtin(pipeline))
        return CMD_BUILTIN;
    return CMD_EXTERNAL;
}

static int is_builtin(t_cmd *pipeline)
{
    const char *cmd_name;
    
    cmd_name = pipeline->argv[0];
    
    return (ft_strcmp(cmd_name, "echo") == 0 ||
            ft_strcmp(cmd_name, "cd") == 0 ||
            ft_strcmp(cmd_name, "pwd") == 0 ||
            ft_strcmp(cmd_name, "env") == 0 ||
            ft_strcmp(cmd_name, "unset") == 0 ||
            ft_strcmp(cmd_name, "exit") == 0 ||
            ft_strcmp(cmd_name, "export") == 0);
}

