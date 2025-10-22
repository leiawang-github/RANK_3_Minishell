/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_edit_mini.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:41:47 by rtiberio          #+#    #+#             */
/*   Updated: 2025/10/22 09:59:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_mini	*mini_new(void)
{
	t_mini	*new;

	new = malloc(sizeof(t_mini));
	if (!new)
		return (NULL);
    ft_memset(new, 0, sizeof(t_mini));
	return (new);
}

void	mini_lstadd_back(t_mini **lst, t_mini *cmd)
{
	t_mini	*last;

	if (!lst || !cmd)
		return ;
	if (!*lst)
	{
		*lst = cmd;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = cmd;
	cmd->next = NULL;
	return ;
}

void	mini_lstdelone(t_mini *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	else//agregar y corregir para eliminar todos los campos de t_mini
	{
		(*del)(lst->cmd_name);
		(*del)(lst->redirs);
		(*del)(lst->cmd_argv);
		(*del)(lst->assigns);
		(*del)(lst->heredoc_delimiter);
		free(lst);
	}
}

void	mini_lstclear(t_mini **lst, void (*del)(void *))
{
	t_mini	*tmp;

	if (!lst || !*lst || !del)
		return ;
	while (lst && *lst)
	{
		tmp = (*lst)->next;
		mini_lstdelone(*lst, del);
		*lst = tmp;
	}
}
