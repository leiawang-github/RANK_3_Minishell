/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 21:02:23 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 11:08:24 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (haystack[i] != '\0' && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && haystack[i + j] != '\0' && i
			+ j < len)
		{
			j++;
			if (needle[j] == 0)
				return ((char *)haystack + i);
		}
		i++;
	}
	return (0);
}

// int main(void)
// {
//     char s1[] = "tomorrow will be a new day";
//     char s2[] = "a";

//     char *result1 = strnstr(s1, s2, 20);
//     char *result2 = ft_strnstr(s1, s2, 20);

//     printf("Strnchr original putput: %s\n", result1);
//     printf("Ft_strnchr putput: %s\n", result2);

//     // printf("result:%s\n", ft_strnstr(((void *)0), "fake", 3));
//     // printf("result:%s\n", strnstr(((void *)0), "fake", 3));
// }