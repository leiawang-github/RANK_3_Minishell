/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:42:32 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:07:51 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(long nbr)
{
	int	len;

	len = 1;
	if (nbr < 0)
		nbr = -nbr;
	while (nbr > 9)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

static void	ft_fillstr(long n, char *dest, int size)
{
	if (n < 0)
	{
		dest[0] = '-';
		n = -n;
		while (size > 0)
		{
			dest[size] = n % 10 + '0';
			n = n / 10;
			size--;
		}
		return ;
	}
	else if (n > 0)
	{
		while (size >= 0)
		{
			dest[size] = n % 10 + '0';
			n = n / 10;
			size--;
		}
		return ;
	}
}

char	*ft_itoa(int n)
{
	int		size;
	char	*dest;

	if (n == 0)
		size = 1;
	else if (n < 0)
		size = int_len(n) + 1;
	else
		size = int_len(n);
	dest = (char *)malloc((size + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	dest[size] = '\0';
	size--;
	if (n == 0)
		dest[0] = '0';
	else
		ft_fillstr(n, dest, size);
	return (dest);
}
