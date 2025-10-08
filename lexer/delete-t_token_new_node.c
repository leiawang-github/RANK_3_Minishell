/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_token_new_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:47:30 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/30 16:23:34 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*trim_quotes(char *str)
// {
// 	char	*res;
// 	int		len;

// 	if (!str)
// 		return (NULL);
// 	len = ft_strlen(str);
// 	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')
// 			|| (str[0] == '\"' && str[len - 1] == '\"')))
// 	{
// 		len = len - 2;
// 		res = malloc (sizeof(char) * (len + 1));
// 		if (!res)
// 			return (NULL);
// 		ft_memcpy(res, str + 1, len);
// 		res[len] = '\0';
// 	}
// 	else
// 		res = ft_strdup(str);
// 	return (res);
// }

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
