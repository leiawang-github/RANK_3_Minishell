/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:42:29 by leia              #+#    #+#             */
/*   Updated: 2025/09/15 22:56:46 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

void error_status(enum e_err kind, int errno_saved)
{
    if (kind == ERR_SYNTAX)
        g_last_status = 2;
    else if (kind == ERR_CMD_NOT_FOUND)
        g_last_status = 127;
    else if (kind == ERR_CANNOT_EXEC)
        g_last_status = 126;
    else if (kind == ERR_SIGINT)
        g_last_status = 130;
    else if (kind == ERR_SIGQUIT)
        g_last_status = 131;
    else
        g_last_status = 1;
}


int ft_errno(const char *file, int saved_errno)
{
    write(2, "minishell: ", 11);
    errno = saved_errno;
    perror(file);
    errno = saved_errno;
    g_last_status = 1;
    return -1;
}

int err_msg(const char *msg)
{
    write(2, "minishell: ", 11);
    write(2, msg, strlen(msg));
    write(2, "\n", 1);
    g_last_status = 1;
    return -1;
}
