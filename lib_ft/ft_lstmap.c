/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 09:39:12 by leia              #+#    #+#             */
/*   Updated: 2024/07/29 10:51:30 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*new_node;
	t_list	*temp;

	newlst = NULL;
	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	while (lst != NULL)
	{
		temp = (t_list *)malloc(sizeof(t_list));
		if (temp == NULL)
		{
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		temp->content = f(lst->content);
		temp->next = NULL;
		if (newlst == NULL)
			newlst = temp;
		else
			new_node->next = temp;
		new_node = temp;
		lst = lst->next;
	}
	return (newlst);
}
