/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:20:33 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:09:34 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*strsub;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		len = 0;
	else if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	i = 0;
	j = start;
	strsub = (char *)malloc((len + 1) * sizeof(char const));
	if (!strsub)
		return (NULL);
	while (i < len && j < ft_strlen(s))
	{
		strsub[i] = s[j];
		i++;
		j++;
	}
	strsub[i] = '\0';
	return (strsub);
}
