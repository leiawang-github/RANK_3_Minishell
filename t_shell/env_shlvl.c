/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_shlvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:58:41 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:06:56 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_shlvl_warning(int lvl)
{
	ft_putstr_fd("minishell: warning: shell level (", 2);
	ft_putnbr_fd(lvl, 2);
	ft_putstr_fd(") too high, resetting to 1\n", 2);
}

t_env	*new_node(const char *name, const char *value)
{
	t_env	*node;
	char	*name_dup;
	char	*value_dup;

	name_dup = ft_strdup(name);
	if (!name_dup)
		return (NULL);
	value_dup = ft_strdup(value);
	if (!value_dup)
	{
		free(name_dup);
		return (NULL);
	}
	node = env_lstnew(name_dup, value_dup);
	if (!node)
	{
		free(name_dup);
		free(value_dup);
		return (NULL);
	}
	return (node);
}

int	check_lvl_value(char *value)
{
	int	lvl;

	lvl = ft_atoi(value);
	lvl++;
	if (lvl < 0)
		lvl = 0;
	else if (lvl > 1000)
		lvl = 1;
	return (lvl);
}

t_env	*find_node(t_env *env_list, char *str)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, str) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

bool	update_shlvl(t_env **env_list)
{
	t_env	*node;
	int		lvl;

	node = find_node(*env_list, "SHLVL");
	if (!node)
	{
		node = new_node("SHLVL", "1");
		if (!node)
			return (false);
		env_lstadd_back(env_list, node);
		return (true);
	}
	lvl = check_lvl_value(node->value);
	free(node->value);
	node->value = ft_itoa(lvl);
	if (!node->value)
		return (false);
	return (true);
}
