/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:30:30 by leia              #+#    #+#             */
/*   Updated: 2025/09/04 19:26:21 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_cmd_type analyze_cmd(t_cmd *cmd) 
{
    if (!cmd->argv || !cmd->argv[0] || !cmd->argv[0][0]) 
    {
        return CMD_REDIR_ONLY; 
    }
    if (is_builtin(cmd->argv[0])) 
    {
        return CMD_BUILTIN;
    }
    return CMD_EXTERNAL;
}

int is_builtin(const char *file)
{
    if (!file)
		return 0;
    return (ft_strcmp(file, "echo")  == 0) ||
           (ft_strcmp(file, "cd")    == 0) ||
           (ft_strcmp(file, "pwd")   == 0) ||
           (ft_strcmp(file, "export")== 0) ||
           (ft_strcmp(file, "unset") == 0) ||
           (ft_strcmp(file, "env")   == 0) ||
           (ft_strcmp(file, "exit")  == 0);
}