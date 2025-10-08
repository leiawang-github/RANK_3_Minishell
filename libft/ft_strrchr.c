/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 19:08:32 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:09:30 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	d;

	i = ft_strlen(s);
	d = c;
	if (s[i] == d)
		return ((char *)&s[i]);
	while (i >= 0)
	{
		if (s[i] == d)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
