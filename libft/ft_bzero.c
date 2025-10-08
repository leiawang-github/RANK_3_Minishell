/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 20:23:46 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:07:31 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*ptr;
	char	c;

	if (n > 0)
	{
		i = 0;
		c = 0;
		ptr = (char *)s;
		while (i < n)
		{
			ptr [i] = c;
			i++;
		}
	}
	return ;
}
