/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 20:46:39 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:07:34 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*cal;

	cal = (void *)malloc(size * count);
	if (!cal)
	{
		return (NULL);
	}
	ft_bzero(cal, count * size);
	return (cal);
}
