/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:42:29 by leia              #+#    #+#             */
/*   Updated: 2025/09/07 15:17:05 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int ft_perror(const char *where)
{
    int saved = errno; /* 保护调用点的 errno */
    if (where)
        dprintf(2, "minishell: %s: %s\n", where, strerror(saved));
    else
        dprintf(2, "minishell: %s\n", strerror(saved));
    errno = saved;     /* 恢复 errno，避免影响上层判断 */
    return -1;
}

int ft_perror_rc(const char *where, int rc)
{
    (void)ft_perror(where);
    return rc;
}

int ft_perror_saved(const char *where, int saved_errno)
{
    if (where)
        dprintf(2, "minishell: %s: %s\n", where, strerror(saved_errno));
    else
        dprintf(2, "minishell: %s\n", strerror(saved_errno));
    errno = saved_errno;
    return -1;
}