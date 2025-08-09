/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:17:52 by leiwang           #+#    #+#             */
/*   Updated: 2024/08/06 10:36:10 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

// int main(void)
// {
// 	printf("Strlen original output: %zu\n", strlen("helloLeia"));
// 	printf("Ft_strlen output: %zu\n", ft_strlen("helloLeiatouch"));
// }