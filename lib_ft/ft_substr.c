/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 18:59:10 by leiwang           #+#    #+#             */
/*   Updated: 2024/09/02 19:38:08 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[len] = '\0';
	return (substr);
}

int main()
{
    char *str = "Hello, world!";
    char *sub;

    sub = ft_substr(str, 7, 5);
    printf("Substring: '%s'\n", sub);
    free(sub);

    sub = ft_substr(str, 20, 5);
    printf("Substring: '%s'\n", sub);
    free(sub);

    sub = ft_substr(str, 7, 20);
    printf("Substring: '%s'\n", sub);
    free(sub);

    sub = ft_substr(str, 7, 0);
    printf("Substring: '%s'\n", sub);
    free(sub);

    return (0);
}
