/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:56:01 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/30 13:53:24 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_substr(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static size_t	substr_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	*alloc_substr(char const *s, char c)
{
	size_t	len;
	char	*substr;

	len = substr_len(s, c);
	substr = (char *)malloc(len + 1);
	if (substr)
	{
		ft_memcpy(substr, s, len);
		substr[len] = '\0';
	}
	return (substr);
}

static void	free_result(char **result, size_t count)
{
	while (count > 0)
		free(result[--count]);
	free(result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	i;

	result = (char **)malloc((ft_count_substr(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			result[i] = alloc_substr(s, c);
			if (!result[i])
			{
				free_result(result, i);
				return (NULL);
			}
			i++;
			s += substr_len(s, c);
		}
	}
	result[i] = NULL;
	return (result);
}
// int	main(void)
// {
// 	char *s = "Hello,,,there, have,  a, great, day";
// 	char c = ',';
// 	char **result;
// 	int i;

// 	result = ft_split(s, c);
// 	if (result == NULL)
// 	{
// 		printf("Memory allocation failed.\n");
// 		return (1);
// 	}

// 	i = 0;
// 	while (result[i])
// 	{
// 		printf("Substring %d: %s\n", i, result[i]);
// 		i++;
// 	}

// 	i = 0;
// 	while (result[i])
// 		free(result[i++]);
// 	free(result);

// 	return (0);
// }