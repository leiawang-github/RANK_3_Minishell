/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 22:55:18 by leiwang           #+#    #+#             */
/*   Updated: 2025/09/15 22:56:08 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

static void print_name_value(const char *name, const char *value);
static int  env_arg_error(const char *arg);

int builtin_env(char **argv, t_env *env_list)
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
