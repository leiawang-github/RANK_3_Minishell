/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 14:21:20 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 11:08:31 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		len;
	char	*tmp;

	tmp = (char *)str;
	len = ft_strlen(str);
	while (len >= 0)
	{
		if (str[len] == (char)c)
			return ((char *)&str[len]);
		len--;
	}
	return (NULL);
}

// int main(void)
// {
//     char str[] = "Hello Leia Wang";

//     printf("Strrchr original output: %s\n", strrchr(str, 'W'));
//     printf("Ft_strrchr output: %s\n", ft_strrchr(str,'W'));
// }