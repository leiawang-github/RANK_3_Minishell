/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:02:49 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:09:27 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_find_trim(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(s1) - 1;
	if (ft_strlen(s1) == 1)
		return (ft_strdup(""));
	while (s1[i] != '\0' && ft_find_trim(s1[i], set) == 1)
		i++;
	while (i < j && ft_find_trim(s1[j], set) == 1)
		j--;
	return (ft_substr(s1, i, j - (i - 1)));
}
