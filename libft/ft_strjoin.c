/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 20:23:26 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:08:59 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	int		i;
	int		j;
	char	*strjoin;

	len = ft_strlen(s1) + ft_strlen(s2);
	strjoin = (char *)malloc((len + 1) * sizeof(char const));
	if (!strjoin)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		strjoin[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		strjoin[i] = s2[j];
		j++;
		i++;
	}
	strjoin[i] = '\0';
	return (strjoin);
}
