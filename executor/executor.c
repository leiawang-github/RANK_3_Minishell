/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:02:10 by leia              #+#    #+#             */
/*   Updated: 2025/09/05 08:32:08 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


int execute_command(t_cmd *pipeline, char **envp)
{
    t_cmd_type cmd_type;
    
    if (!pipeline)
        return (0);
    if(!pipeline->next) // single command
    {
        cmd_type = analyze_cmd(pipeline);
        {
            if (cmd_type == CMD_REDIR_ONLY) //单节点只有重定向
                return exec_redir_only(pipeline);
            if (cmd_type == CMD_BUILTIN) 
                return exec_builtin(pipeline, envp); //这里面还要分：有重定向和无重定向
            if (cmd_type == CMD_EXTERNAL)
                return exec_single_external(pipeline, *envp); //外部命令，里面也要分：有重定向和无重定向
        }
        return (127);
    }
    return
        exec_pipeline(pipeline, envp);
}
    









	 





	

	

