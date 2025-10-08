/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:20:44 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:08:23 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*ptrd;
	char	*ptrs;

	i = 0;
	ptrd = (char *)dst;
	ptrs = (char *)src;
	while (i < n && (ptrd != NULL || ptrs != NULL))
	{
		ptrd[i] = ptrs[i];
		i++;
	}
	return (dst);
}
