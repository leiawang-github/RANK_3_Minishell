/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:17:41 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 11:06:43 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*dst_p;
	const char	*src_p;
	size_t		i;

	dst_p = (char *)dst;
	src_p = (char *)src;
	i = 0;
	if (!dst_p && !src_p)
		return (NULL);
	while (i < n)
	{
		dst_p[i] = src_p[i];
		i++;
	}
	return (dst);
}

// int main(void)
// {
//  const char *src = "some good news";
//  char dst1[20];
//  char dst2[20];

//  memcpy(dst1, src, 8);
//  ft_memcpy(dst2, src, 8);

//  printf("Memcpy original output: %s\n", dst1);
//  printf("Ft_memcpy output: %s\n", dst2);

// }
