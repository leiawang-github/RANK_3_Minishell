/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 14:13:35 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 18:15:15 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

static int unset_env_var(const char *name, t_env **env_list);

int builtin_unset(char **argv, t_env **env_list)
{
    int argc;
    int result;
    int i;

    argc = 0;
    result = 0;
    i = 1;

    while (argv[argc])
        argc++;
    if (argc == 1)
        return 0;
    while (i < argc)
    {
        if (unset_env_var(argv[i], env_list) != 0)
            result = 1;
        i++;
    }
    return (result);
}

int is_valid_var_name(const char *name)
{
    int i;

    i = 1;
    if (!name || !name[0])
        return (0);
    if (!ft_isalpha(name[0]) && name[0] != '_') return 0;
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
        i++;
    }
    return 1;
}

static int error_handler(const char *name)
{
    write(2, "\nminishell: unset: `", 19);
    write(2, name, ft_strlen(name));
    write(2, "': not a valid identifier\n", 28);
    return 1;
}

static int unset_env_var(const char *name, t_env **env_list)
{
    t_env *current = *env_list;
    t_env *prev = NULL;

    if (!is_valid_var_name(name))
        error_handler(name);
    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_list = current->next;
            free(current->name);
            if (current->value)
                free(current->value);
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}
