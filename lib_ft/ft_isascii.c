/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:19:45 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 10:52:18 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	return (0 <= c && c <= 0177);
}

// int main(void)
// {
// 	printf("the result will be: %d\n", ft_isascii('0178'));
// 	printf("the result will be: %d\n", isascii('0178'));
// }
