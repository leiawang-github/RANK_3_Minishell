/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:35:05 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 23:04:02 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

// minishell subject : cd with only a relative or absolute path
/*how to achieve
要点（最小实现版）：

1. 只接受一个参数：cd path。多于/少于一参数直接报错返回 1。
2. chdir(path) 失败按 perror 风格报错，返回 1。
3. 成功后尽量更新 PWD/OLDPWD（用 getcwd，失败可忽略对应更新），最后返回 0。
不处理 cd -、无参数跳 HOME、CDPATH、逻辑/物理路径模式等扩展行为。

*/

int	mini_cd(char **argv, t_env *env_list)
{
	int	argc;
	char	*target;
	char	*oldpwd;

	argc = count_args(argv);
	if (argc != 2)
		return (err_msg("cd", ": exactly one argument required", ERR_SYS_BUILTIN));
	target = argv[1];
	if (!target || target[0] == '\0')
		return (err_msg("cd", ": invalid path", ERR_SYS_BUILTIN));
	oldpwd = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		if (oldpwd)
			free(oldpwd);
		return (ft_errno("cd", errno, ERR_SYS_BUILTIN));
	}
	update_pwd_vars(env_list, oldpwd);
	g_last_status = 0;
	return (0);
}

static int	count_args(char **argv)
{
	int n;

	n = 0;
	while (argv && argv[n])
		n++;
	return (n);
}

static void	update_pwd_vars(t_env *env_list, char *oldpwd)
{
	char	*newpwd;
	int		rc;

	newpwd = getcwd(NULL, 0);
	if (oldpwd)
	{
		rc = env_set(env_list, "OLDPWD", oldpwd);
		(void)rc;
		free(oldpwd);
	}
	if (newpwd)
	{
		rc = env_set(env_list, "PWD", newpwd);
		(void)rc;
		free(newpwd);
	}
}
