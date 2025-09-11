/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 18:48:08 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:07:01 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(char *env_line)
{
	char	**tmp;
	t_env	*new_node;
	char	*name;
	char	*value;

	tmp = ft_split(env_line, '=');
	if (!tmp || !tmp[0])
		return (free_array(tmp));
	name = ft_strdup(tmp[0]);
	if (!name)
		return (free_array(tmp));
	if (tmp[1] == NULL)
		value = ft_strdup("");
	else
		value = ft_strdup(tmp[1]);
	if (!value)
	{
		free(name);
		return (free_array(tmp));
	}
	new_node = env_lstnew(name, value);
	free_array(tmp);
	return (new_node);
}

t_env	*create_env(char **env)
{
	int		i;
	t_env	*env_copy;
	t_env	*new_node;
	t_env	*last;

	i = 0;
	env_copy = NULL;
	last = NULL;
	while (env[i] != NULL)
	{
		new_node = create_env_node(env[i]);
		if (!new_node)
		{
			env_lstclear(&env_copy, &free_null);
			return (NULL);
		}
		if (!env_copy)
			env_copy = new_node;
		else
			last->next = new_node;
		last = new_node;
		i++;
	}
	return (env_copy);
}

t_env	*env_handling(char **env)
{
	t_env	*env_list;

	env_list = create_env(env);
	if (!env_list)
		return (NULL);
	if (!update_shlvl(&env_list))
	{
		env_lstclear(&env_list, &free_null);
		return (NULL);
	}
	return (env_list);
}
