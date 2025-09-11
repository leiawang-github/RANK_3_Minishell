/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_implementation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:54:52 by leia              #+#    #+#             */
/*   Updated: 2025/09/10 21:43:14 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int is_flag_n(const char *s)
{
    int i;

    if (!s || s[0] != '-' || s[1] == '\0')
        return 0;
    i = 1;
    while (s[i] == 'n')
        i++;
    return s[i] == '\0'; /* 形如 -n, -nn, -nnn */
}

int builtin_echo(char **argv)
{
    int i;
    int newline;
    
    i = 1;
    newline = 1; 
    while (argv[i] && is_flag_n(argv[i]))
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

int builtin_export(char **argv)
{
    


    
}

int builtin_unset(char **argv)
{
    
}

int builtin_env(void)
{
        


    
}

int builtin_exit(char **argv)
{
    (void)argv;  
    write(1, "exit\n", 5);       
    exit(g_last_status);      
    return 0; 
}



