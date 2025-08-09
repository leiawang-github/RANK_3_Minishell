/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:27:35 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 11:06:46 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t len)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = str;
	while (i < len)
	{
		p[i] = c;
		i++;
	}
	return (str);
}

// int main(void)
// {
// 	char str[] = "Hello Leia";
// 	char c = '0';

// 	printf("Memset original output: %s\n", memset(str, c, 5));
// 	printf("Ft_memset output:  %s\n", ft_memset(str, c, 5));
// }
