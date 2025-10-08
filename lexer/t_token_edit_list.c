/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_edit_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:21:47 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/06 19:31:46 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
	new->next = NULL;
	return ;
}

void	token_lstdelone(t_token *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	else
	{
		(*del)(lst->value);
		free(lst);
	}
}

void	token_lstclear(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;

	if (!lst || !*lst || !del)
		return ;
	while (lst && *lst)
	{
		tmp = (*lst)->next;
		token_lstdelone(*lst, del);
		*lst = tmp;
	}
}

t_token	*token_lstnew(char *value, t_token_type type)
{
	t_token	*new;
	char	*final_value;

	final_value = ft_strdup(value);
	if (!final_value)
		return (NULL);
	new = malloc(sizeof(t_token));
	if (!new)
	{
		free (final_value);
		return (NULL);
	}
	new->value = final_value;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_token	*token_node_create(char *token)
{
	t_token			*new;
	t_token_type	type;

	if (!token)
		type = type_token(NULL);
	else
		type = type_token(token);
	new = token_lstnew(token, type);
	return (new);
}
