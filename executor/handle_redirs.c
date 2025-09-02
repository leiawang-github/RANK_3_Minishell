/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:44:21 by leia              #+#    #+#             */
/*   Updated: 2025/09/02 13:50:46 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


int has_redir(t_cmd *cmd) 
{
    t_cmd *current = cmd;
    
    while (current) {
        if (current->redirs != NULL) {
            return 1;
        }
        current = current->next;
    }
    
    return 0;
}

void handle_redirects(t_cmd *cmd) {
    // 不管什么命令类型，重定向处理逻辑是一样的
    setup_redirects(cmd);
    apply_redirects(cmd);
}