/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 21:19:44 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 12:35:11 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	i = 0;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	while (src[i] != '\0' && dst_len + i < dstsize - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

// int	main(void)
// {
// 	char	dst[100] = "How are you doing?!";
// 	char	src[10] = "great!";

// 	printf("dst before strlcat: %s\n", dst);
// 	printf("Strlcat original output: %zu\n", strlcat(dst, src, 5));
// 	// printf("Ft_strlcat output: %zu\n", ft_strlcat(dst, src, 5));
// 	printf("dst after strlcat: %s\n", dst);
// 	return (0);
// }
