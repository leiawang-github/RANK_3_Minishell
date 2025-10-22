/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 18:48:08 by rtiberio          #+#    #+#             */
/*   Updated: 2025/10/21 12:23:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_nodes(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	name_and_value_length(char *name, char *value)
{
	int	length;

	length = 0;
	if (name)
		length = length + ft_strlen(name);
	if (value)
		length =  length + ft_strlen(value);
	return (length);
}

void construct_env_string(char *dest, char *name, char *value)
{
	int i;
	int j;

	i = 0;
	while (name && name[i] != '\0')
	{
		dest[i] = name[i];
		i++;
	}
	dest[i] = '=';
	i++;
	j = 0;
	while (value && value[j] != '\0')
	{
		dest[i] = value[j];
		i++;
		j++;
	}
	dest[i] = '\0';
}

char **list_to_array(t_env *env_list)
{
	char	**new_envp;
	int		size;
	int		str_len;
	int		i;
	
	size = count_env_nodes(env_list);
	new_envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (env_list != NULL)
	{
		if (env_list->name = NULL)
			return (free_array(new_envp));
		str_len = name_and_value_length(env_list->name, env_list->value) + 1; // because of '='
		new_envp[i] = malloc(sizeof(char) * (str_len + 1));
		if (!new_envp[i])
			return (free_array(new_envp));
		construct_env_string(new_envp[i], env_list->name, env_list->value);
		i++;
		env_list = env_list->next;
	}
	new_envp[i]	= NULL;
	return (new_envp);	
}
