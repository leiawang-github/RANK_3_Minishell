/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_in_parent.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:54:52 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 22:48:08 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

int	run_builtin_in_parent(char **argv, t_env *env_list)
{
	if (!argv || !argv[0])
		return (0);
	if (ft_strcmp(argv[0], "cd") == 0)
		return (mini_cd(argv, env_list));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (mini_unset(argv, env_list));
	if (ft_strcmp(argv[0], "export") == 0)
		return (mini_export(argv, env_list));
    if (ft_strcmp(argv[0], "exit") == 0)
    {
        mini_exit(argv);
        return (0);
    }
	return (0);
}

int mini_exit(char **argv)
{
    (void)argv;
    write(1, "exit\n", 5);
    exit(g_last_status);
    return 0;
}



