/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 09:56:15 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 11:53:56 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = 0;
		i++;
	}
}

// int main(void)
// {
//     char str1[] = "What a beautiful day!";
//     char str2[] = "What a beautiful day!";
//     bzero(str1, 3);
//     ft_bzero(str2, 3);
//     printf("Original bzero output: %s\n", str1 + 3);
//     printf("Ft_bzero output: %s\n", str2 + 3);
//     return (0);
// }
