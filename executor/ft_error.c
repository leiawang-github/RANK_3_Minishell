/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:42:29 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 23:24:25 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

void error_status(enum e_err kind)
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

/*
when system call fails, the kernal set errno by perror()

void perror(const char *s);
extern int errno;  // 全局错误号变量


// 常见的 errno 值
#define ENOENT    2   // No such file or directory
#define EACCES   13   // Permission denied
#define EINVAL   22   // Invalid argument
#define EISDIR   21   // Is a directory
#define ENOTDIR  20   // Not a directory
#define ENAMETOOLONG 36  // File name too long
*/

int ft_errno(const char *file, int saved_errno, enum e_err kind)
{
    write(2, "minishell: ", 11);
    // 不修改全局errno，使用perror的替代实现
    write(2, file, ft_strlen(file));
    write(2, ": ", 2);
    write(2, strerror(saved_errno), ft_strlen(strerror(saved_errno)));
    write(2, "\n", 1);

    error_status(kind);
    return -1;
}

/* Note：支持两个字符串拼接，避免内存分配
Shell 内部逻辑错误才调用这个函数， shell级别的

*/
int err_msg(const char *prefix, const char *suffix, enum e_err kind)
{
    write(2, "minishell: ", 11);
    write(2, prefix, ft_strlen(prefix));
    write(2, suffix, ft_strlen(suffix));
    write(2, "\n", 1);
    error_status(kind);
    return -1;
}

