/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:57:21 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 10:58:08 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)(s + i));
		i = i + 1;
	}
	return (NULL);
}

// int main(void)
// {
//     char *src = "/|\x12\xff\x09\0\x42\042\0\42|\\";
//     char c = '\\';

//     printf("Memchr original ouptut: %d\n", memchr(src, c, 10));
//     printf("Ft_memchr ouptut: %d\n", ft_memchr(src, c, 10));
// }

// NOTES
// int main(void)
// {
//     char strarr1[] = "hello"; //with a '\0'
//     char strarr2[] = {'h','e','l','l','o'}; //is add '\0', will be the same

//     char *substr1 = memchr(strarr1, 'l', 5);
//     char *substr2 = memchr(strarr2, 'l', 5);

//     printf("string output: %s\n", substr1);
//     printf("string output: %c%c%c\n", substr2[0],substr2[1],substr2[2]);
// will be same output
// }
