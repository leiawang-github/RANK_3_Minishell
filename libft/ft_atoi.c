/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 20:26:12 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:07:27 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	result_atoi(int sign)
{
	if (sign == 1)
		return (-1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	r;

	i = 0;
	sign = 1;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	r = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (r > (LONG_MAX - (str[i] - '0')) / 10)
			return (result_atoi(sign));
		r = r * 10 + str[i] - '0';
		i++;
	}
	return (r * sign);
}
