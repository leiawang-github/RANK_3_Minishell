/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:04:23 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:09:05 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	srcsize;
	char	*d;
	char	*s;

	s = (char *)src;
	d = (char *)dst;
	i = 0;
	srcsize = ft_strlen(src);
	if (dstsize > 0)
	{
		while (src[i] != '\0' && i < (dstsize - 1))
		{
			d[i] = s[i];
			i++;
		}
		d[i] = '\0';
	}
	return (srcsize);
}
