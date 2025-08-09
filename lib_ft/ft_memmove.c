/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 09:48:22 by leia              #+#    #+#             */
/*   Updated: 2024/07/29 20:06:36 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	i = 0;
	if ((char *)dst == (char *)src || len == 0)
		return ((void *)dst);
	if ((char *)dst < (char *)src || (char *)dst >= ((char *)src + len))
	{
		while (i < len)
		{
			((char *)dst)[i] = ((char *)src)[i];
			i++;
		}
	}
	else
	{
		i = len;
		while (i > 0)
		{
			i--;
			((char *)dst)[i] = ((char *)src)[i];
		}
	}
	return ((void *)dst);
}

// int main(void)
// {
//     char dst[] = "happy holiday";
//     char src[] = "when it shines";

//     //printf("Memmove original output: %s\n",memmove(dst, src, 10));
//     printf("Ft_memmove output: %p\n",ft_memmove(dst, src, 10));

//     return (0);
// }
