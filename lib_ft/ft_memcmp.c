/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:10:35 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 11:10:37 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*s1_ptr;
	const unsigned char	*s2_ptr;

	i = 0;
	s1_ptr = s1;
	s2_ptr = s2;
	while (i < n)
	{
		if (s1_ptr[i] != s2_ptr[i])
			return (s1_ptr[i] - s2_ptr[i]);
		i++;
	}
	if (i < n)
	{
		if (s1_ptr[i] != s2_ptr[i])
			return (s1_ptr[i] - s2_ptr[i]);
	}
	return (0);
}

// int main(void)
// {
//     const char s1[] = "abcdefghij";
//     const char s2[] = "abcdefgxyz";

//     printf("Memcmp original output: %d\n", memcmp(s1, s2, 7));
//     printf("Ft_memcmp output: %d\n", ft_memcmp(s1, s2, 7));
// }
