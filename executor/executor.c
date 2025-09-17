/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:02:10 by leia              #+#    #+#             */
/*   Updated: 2025/09/16 20:39:08 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int execute_command(t_cmd *pipeline, char **envp, t_env *env_list)
{
    int rc;
    int interrupted;
    t_cmd_type cmd_type;

    rc = 0;
    interrupted = 0;
    rc = preprocess_heredoc(pipeline, envp, &interrupted);
    if (rc != 0)
        return rc; /* 130 if interrupted, else error */
    if (!pipeline)
        return 0;
    if (!pipeline->next) //单节点
    {
        cmd_type = analyze_cmd(pipeline);
        if (cmd_type == CMD_REDIR_ONLY)
            return exec_redir_only(pipeline);
        if (cmd_type == CMD_BUILTIN)
            return  exec_builtin_in_single_cmd(pipeline, env_list);
        if (cmd_type == CMD_EXTERNAL)
            return exec_single_external(pipeline, env_list, envp);
        return 127;
    }
    /* pipeline */
    return exec_pipeline(pipeline, envp, env_list);
}


