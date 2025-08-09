/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 09:01:15 by leia              #+#    #+#             */
/*   Updated: 2024/07/29 17:42:07 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	get_n_len(long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n > 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*result;
	long	num;
	int		n_len;

	num = n;
	n_len = get_n_len(num);
	result = (char *)malloc(sizeof(char) * (n_len + 1));
	if (!result)
		return (NULL);
	result[n_len] = '\0';
	if (n == 0)
		result[0] = '0';
	if (num < 0)
	{
		result[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		result[--n_len] = (num % 10) + '0';
		num /= 10;
	}
	return (result);
}

// int	main(void)
// {
// 	int		n;
// 	char	*str_num;

// 	n = -3452;
// 	str_num = ft_itoa(n);
// 	if (str_num != NULL)
// 	{
// 		printf("String: %s\n", str_num);
// 		free(str_num);
// 	}
// 	return (0);
// }
