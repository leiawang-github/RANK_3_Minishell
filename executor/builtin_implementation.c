/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_implementation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:54:52 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 18:24:01 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

static int is_n_option(const char *n_option);

static int	has_dash_option(char **argv)
{
	int i;

	if (!argv || !argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-')
			return (1);
		i++;
	}
	return (0);
}

int	run_builtin_in_parent(char **argv, t_env *env_list)
{
	if (!argv || !argv[0])
		return (0);
	if (ft_strcmp(argv[0], "cd") == 0)
		return (mini_cd(argv, env_list));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (mini_unset(argv, env_list));

	if (ft_strcmp(argv[0], "exit") == 0)
	{
		mini_exit(argv, env_list);
		return (0); /* 不会到这里；为了静态检查留一返回 */
	}

	/* export（no options）：有参数或非法选项 → 父进程；无参数仅打印 */
	if (ft_strcmp(argv[0], "export") == 0)
	{
		if (has_dash_option(argv))
			return (mini_export(argv, env_list));
		if (argv[1] != NULL)
			return (mini_export(argv, env_list));
		/* 无参数：如果你顶层把它路由到子进程，这里一般不会进来；
		   若进来，也允许在父进程打印环境并返回 0： */
		return (mini_export(argv, env_list));
	}
	/* 其他内建（echo/pwd/env）不该走到父进程；返回 0 保守处理 */
	return (0);
}

int builtin_echo(char **argv)
{
    int i;
    int newline;

    i = 1; // here argv[0] should be "echo", but we only focus what's after echo
    newline = 1; // we set default a newline, this is a flag
    while (argv[i] && is_n_option(argv[i])) // if we found n_option of the number i argv
    {
        newline = 0; //dont give a newline
        i++; //we just move onward to next argv
    }
    while (argv[i])
    {
        write(1, argv[i], ft_strlen(argv[i]));
        if (argv[i + 1])
            write(1, " ", 1); // if the next argv exists we put a space
        i++;
    }
    if (newline)
        write(1, "\n", 1);
    return 0;
}

static int is_n_option(const char *n_option)
{
    int i;

    if (!n_option || n_option[0] != '-' || n_option[1] == '\0') //should be '-' without space , so '-   n' is not ok
        return 0;
    i = 1; //check after '-', see if there is n or many ns like -nn, -nnnnn
    while (n_option[i] == 'n')
        i++;
    return n_option[i] == '\0'; // if the const char end with '\0', means it is a valid n_string
}


int builtin_pwd(void)
{
    char buf[PATH_MAX];

    if (getcwd(buf, sizeof(buf)) != NULL)
    {
        printf("%s\n", buf);
        return 0;
    }
    ft_errno("pwd", errno, ERR_SYS_BUILTIN); // recheck needed
    return 1;
}

int builtin_exit(char **argv)
{
    (void)argv;
    write(1, "exit\n", 5);
    exit(g_last_status);
    return 0;
}



