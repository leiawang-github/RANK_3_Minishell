/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:53:50 by leia              #+#    #+#             */
/*   Updated: 2025/06/24 19:14:46 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		s_len;
	char	*result;

	i = 0;
	s_len = ft_strlen(s);
	result = (char *)malloc(sizeof(char) * (s_len + 1));
	if (result == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		result[i] = f(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}

char ft_toupper1(unsigned int index, char c) {
	(void) index;
    if ('a' <= c && c <= 'z') {
        return (c - 32);
    }
    return (c);
}

int main(void) {
    char s[] = "hello world";
    char *result = ft_strmapi(s, ft_toupper1);

    if (result != NULL) {
        printf("Output: %s\n", result);
    free(result);
    }
    return (0);
}
