/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:41:47 by rtiberio          #+#    #+#             */
/*   Updated: 2025/10/22 10:01:57 by codespace        ###   ########.fr       */
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
	return (n_pairs);
}

char	*ft_remove_quotes(char *tmp)
{
	int		len;
	char	*clean;
	int		n_pairs;

	if (!tmp)
		return (NULL);
	n_pairs = count_npairs_quotes(tmp);
	if (n_pairs == 0)
		clean = ft_strdup(tmp);
	else
	{
		len = ft_strlen(tmp) - (2 * n_pairs);
		clean = malloc(sizeof(char) * (len + 1));
		if (!clean)
			return (NULL);
		clean [len] = '\0';
		ft_clean_quotes(clean, tmp);
	}
	return (clean);
}
