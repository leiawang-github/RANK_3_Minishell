/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 01:40:21 by leiwang           #+#    #+#             */
/*   Updated: 2025/10/24 01:45:56 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

/*
                 run_external(argv, envp)
                           |
                           v
                   [ argv[0] 为空? ]
                          / \
                        是   否
                        |     v
                        |  [ 含'/'? ]───是──▶ execve(argv[0],...) 成功不返回
                        |     | 否
                        v     v
                 （直接返回） [ 取 PATH ]
                               |
                     [ PATH 为空/缺失? ]
                               |
                         是 ──▶ errno=ENOENT；返回
                               |
                               否
                               v
                 ┌──────── 遍历 PATH 段落 ─────────┐
                 |  dir := next_segment(PATH, ':')  |
                 |  full := dir + "/" + argv[0]     |
                 |  execve(full, argv, envp)        |
                 |  若成功→不返回                   |
                 |  若失败→继续下一个 dir          |
                 └──────────────────────────────────┘
                               |
                               v
                     [ 全部尝试均失败 ]
                               |
                 （保留 execve 的 errno；若为0→ENOENT）
                               |
                              返回

*/

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h> /* strncmp */

void	run_external(char **argv, char **envp)
{
	char *path;

	if (!argv || !argv[0] || argv[0][0] == '\0')
		return ;
	if (has_slash(argv[0]))
	{
		execve(argv[0], argv, envp); /* 成功不返回 */
		return ;
	}
	path = get_env_value(envp, "PATH");
	if (!path || path[0] == '\0')
	{
		errno = ENOENT;
		return ;
	}
	try_exec_in_path(path, argv, envp);
	if (errno == 0)
		errno = ENOENT;
}

/* 取环境变量值：返回 key= 后面的指针；没有则 NULL */
static char	*get_env_value(char **envp, const char *key)
{
	int		i;
	size_t	k;

	if (!envp || !key)
		return (NULL);
	k = 0;
	while (key[k])
		k++;
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, k) && envp[i][k] == '=')
			return (envp[i] + k + 1);
		i++;
	}
	return (NULL);
}

static int	has_slash(const char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	*path_join_cmd(const char *dir, const char *cmd)
{
	char	*base;
	char	*full;

	if (!cmd || *cmd == '\0')
		return (NULL);
	if (!dir || *dir == '\0')
		base = ft_strjoin(".", "/");
	else
		base = ft_strjoin(dir, "/");
	if (!base)
		return (NULL);
	full = ft_strjoin(base, cmd);
	free(base);
	return (full);
}

static void	try_exec_in_path(const char *path, char **argv, char **envp)
{
	const char	*p;
	const char	*q;
	char		save;
	char		*full;

	if (!path || !*path)
		return ;
	p = path;
	while (*p)
	{
		q = p;
		while (*q && *q != ':')
			q++;
		save = *q;
		*(char *)q = '\0';
		full = path_join_cmd(p, argv[0]);
		*(char *)q = save;
		if (full)
		{
			execve(full, argv, envp); /* 成功不返回 */
			free(full);
		}
		if (*q == ':')
			p = q + 1;
		else
			p = q;
	}
}


