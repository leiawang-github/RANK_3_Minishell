/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_implementation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:54:52 by leia              #+#    #+#             */
/*   Updated: 2025/09/14 12:07:21 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/env_copy.h"
#include "../include/executor.h"

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

int builtin_echo(char **argv)
{
    int i;
    int newline;
    
    i = 1;
    newline = 1; // we set default a newline, this is a flag
    while (argv[i] && is_n_option(argv[i]))
    {
        newline = 0;
        i++;
    }
    while (argv[i])
    {
        write(1, argv[i], ft_strlen(argv[i]));
        if (argv[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (newline)
        write(1, "\n", 1);
    return 0;
}


int builtin_pwd(void)
{
    char buf[PATH_MAX];

    if (getcwd(buf, sizeof(buf)) != NULL)
    {
        printf("%s\n", buf);
        return 0;
    }
    ft_errno("pwd", errno); // recheck needed
    return 1;
}


int builtin_cd(char **argv, t_env *env)
{
    char *newpwd;
    
    if (!argv[1])
        return err_msg("cd: missing argument");
    if (argv[2])
        return err_msg("cd: too many arguments"); 
    if (argv[1][0] == '\0')
        return err_msg("cd: empty path");
    if (chdir(argv[1]) != 0) 
        return ft_errno("cd",errno);
    newpwd = getcwd(NULL, 0);
    if (!newpwd)
    {
        err_msg("cd: warning: failed to update PWD");
        g_last_status = 0;
        return 0;
    } 
    if (env)
        set_env_var(env, "PWD", newpwd);
    free(newpwd);
    g_last_status = 0;    
    return (0);
}

int builtin_env(char **argv, t_env *env_list)
{
    t_env *cur;

    if (argv && argv[1] != NULL)
        return env_arg_error(argv);

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

int builtin_exit(char **argv)
{
    (void)argv;  
    write(1, "exit\n", 5);       
    exit(g_last_status);      
    return 0; 
}



