/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_implementation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:54:52 by leia              #+#    #+#             */
/*   Updated: 2025/09/15 22:56:16 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/env_copy.h"
#include "../include/executor.h"

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



