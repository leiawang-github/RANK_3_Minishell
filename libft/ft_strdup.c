/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:40:52 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:08:53 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	int		len;
	int		i;

	len = 0;
	while (s1[len] != '\0')
		len++;
	copy = (char *)malloc((len + 1) * sizeof(char));
	if (!copy)
	{
		errno = ENOMEM;
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}
