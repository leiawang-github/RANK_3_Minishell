/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 09:15:10 by leia              #+#    #+#             */
/*   Updated: 2024/07/29 10:55:15 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*given_node;

	current = *lst;
	if (lst == NULL || del == NULL)
		return ;
	while (current != NULL)
	{
		given_node = current->next;
		del(current->content);
		free(current);
		current = given_node;
	}
	*lst = NULL;
}
