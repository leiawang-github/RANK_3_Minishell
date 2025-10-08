/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 19:14:42 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:09:19 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
	{
		return (0);
	}
	while (s1[i] == s2[i] && s1[i] != '\0' && i < (n - 1))
	{
		i++;
	}
	if (s1[i] == s2[i])
	{
		return (0);
	}
	else
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
