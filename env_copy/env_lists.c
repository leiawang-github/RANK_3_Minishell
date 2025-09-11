/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lists.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:28:45 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/12 10:21:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_lstnew(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	if (!name || !value)
	{
		new->name = NULL;
		new->value = NULL;
		new->next = NULL;
	}
	else
	{
		new->name = name;
		new->value = value;
		new->next = NULL;
	}
	return (new);
}

void	env_lstdelone(t_env *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	else
	{
		(*del)(lst->name);
		(*del)(lst->value);
		free(lst);
	}
}

void	env_lstclear(t_env **lst, void (*del)(void *))
{
	t_env	*tmp;

	if (!lst || !*lst || !del)
		return ;
	while (lst && *lst)
	{
		tmp = (*lst)->next;
		env_lstdelone(*lst, del);
		*lst = tmp;
	}
}

t_env	*env_lstlast(t_env *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	env_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = env_lstlast(*lst);
		last->next = new;
	}
	else
		*lst = new;
}
