/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 21:41:12 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:08:21 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	*ptr;

	i = 0;
	str = (unsigned char *)s1;
	ptr = (unsigned char *)s2;
	while (i < n)
	{
		if (str[i] != ptr[i])
			return ((str[i] - ptr[i]));
		i++;
	}
	return (0);
}
