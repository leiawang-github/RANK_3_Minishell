/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_in_child.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 22:55:18 by leiwang           #+#    #+#             */
/*   Updated: 2025/10/24 01:38:42 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

int	is_child_builtin(t_mini *cmd)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	name = cmd->argv[0];
	if (!ft_strcmp(name, "echo"))
		return (1);
	if (!ft_strcmp(name, "pwd"))
		return (1);
	if (!ft_strcmp(name, "env"))
		return (1);
	return (0);
}

int	run_builtin_child(t_mini *cmd, t_env *env_list)
{
	char	**argv;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	argv = cmd->argv;
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(argv,env_list));
	return (127);
}

int mini_env(char **argv, t_env *env_list)
{
    t_env *cur;

    if (argv && argv[1] != NULL)
        return env_arg_error(argv[1]);

    cur = env_list;
    while (cur)
    {
        if (cur->value != NULL)
            print_name_value(cur->name, cur->value);
        cur = cur->next;
    }
    return 0;
}

static void print_name_value(const char *name, const char *value)
{
    if (!name || !value)
        return;
    printf("%s", name);
    printf("=");
    printf("%s\n",value);
}

static int  env_arg_error(const char *arg)
{
    write(2, "minishell: env: ", 16);
    if (arg && *arg)
        write(2, arg, ft_strlen(arg));
    else
        write(2, "(null)", 6);
    write(2, ": No such file or directory\n", 28);
    return 127;
}

int mini_pwd(void)
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

int mini_echo(char **argv)
{
    int i;
    int newline;

    i = 1;
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
