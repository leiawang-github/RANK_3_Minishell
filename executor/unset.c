/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 14:13:35 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 22:29:07 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

int	mini_unset(char **argv, t_env **envp)
{
	int	argc;
	int	i;
	int	result;

	argc = 0;
	while (argv && argv[argc])
		argc++;
	if (argc == 1)
	{
		g_last_status = 0;
		return (0);
	}
	i = 1;
	result = 0;
	while (i < argc)
	{
		if (unset_env_var(argv[i], envp) != 0)
			result = 1;
		i++;
	}
	if (result != 0)
		error_status(ERR_SYS_BUILTIN);
	else
		g_last_status = 0;
	return (result);
}

static int	unset_env_var(const char *name, t_env **envp)
{
	t_env	*cur;
	t_env	*prev;

	if (!is_valid_var_name(name))
		return (unset_ident_error(name));
	cur = *envp;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*envp = cur->next;
			if (cur->name)
				free(cur->name);
			if (cur->value)
				free(cur->value);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

int	is_valid_var_name(const char *name)
{
	int i;

	if (!name)
		return (0);
	if (!name[0])
		return (0);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (name[i] == '=')
			return (0);
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	unset_ident_error(const char *name)
{
	write(2, "minishell: unset: `", 20);
	if (name)
		write(2, name, ft_strlen(name));
	write(2, "': not a valid identifier\n", 27);
	error_status(ERR_SYS_BUILTIN);
	return (1);
}
