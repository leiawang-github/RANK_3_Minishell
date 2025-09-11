/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:30:30 by leia              #+#    #+#             */
/*   Updated: 2025/09/07 21:27:33 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_cmd_type analyze_cmd(t_cmd *cmd) 
{
    if(!cmd)
        return CMD_INVALID;
    if (!cmd->argv || !cmd->argv[0]) 
    {
        if(cmd->redirs)
            return CMD_REDIR_ONLY; 
        return CMD_INVALID;
    }
    if (is_builtin(cmd->argv[0])) 
    {
        return CMD_BUILTIN;
    }
    return CMD_EXTERNAL;
}
