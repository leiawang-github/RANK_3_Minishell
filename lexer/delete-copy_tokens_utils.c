/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete-copy_tokens_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 09:04:43 by codespace         #+#    #+#             */
/*   Updated: 2025/09/20 19:40:10 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_clean_quotes(char *dst, char *src)
{
	int		i;
	int		j;
	char	q;

	i = 0;
	j = 0;
	q = 0;
	while (src[i] != '\0')
	{
		if (is_quote(src[i]) == true && q == 0)
			q = src[i];
		else if (q == src[i])
			q = 0;
		else
		{
			dst[j] = src[i];
			j++;
		}
		i++;
	}
	return (dst);
}

int	count_npairs_quotes(char *tmp)
{
	int		i;
	int		n_pairs;
	char	q;
	int		len;

	i = 0;
	n_pairs = 0;
	q = 0;
	while (tmp[i] != '\0')
	{
		if (is_quote(tmp[i]) == true)
		{
			if (q == 0)
				q = tmp[i];
			else if (tmp[i] == q)
			{
				q = 0;
				n_pairs++;
			}
		}
		i++;
	}
	len = ft_strlen(tmp) - (2 * n_pairs);
	return (len);
}

char	*ft_remove_quotes(char *tmp)
{
	int		len;
	char	*clean;

	if (!tmp)
		return (NULL);
	len = count_npairs_quotes(tmp);
	clean = malloc(sizeof(char) * (len + 1));
	if (!clean)
		return (NULL);
	clean [len] = '\0';
	ft_clean_quotes(clean, tmp);
	return (clean);
}
