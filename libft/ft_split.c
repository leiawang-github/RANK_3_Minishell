/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 18:11:14 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/16 19:36:03 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && (s[i + 1] == c || !s[i + 1]))
			count++;
		i++;
	}
	return (count);
}

static void	ft_copy(char const *s, char *dest, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
}

static char	**ft_free(char **split, int z)
{
	z = z - 1;
	while (z >= 0)
	{
		free(split[z]);
		z--;
	}
	free (split);
	return (NULL);
}

static char	**ft_create_array(char **split, char const *s, char c)
{
	int	i;
	int	j;
	int	z;

	i = 0;
	z = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			j = 0;
			while (s[i + j] != c && s[i + j] != '\0')
				j++;
			split[z] = malloc(sizeof(char) * (j + 1));
			if (!split[z])
				return (ft_free(split, z));
			ft_copy(s + i, split[z], c);
			i = i + j;
			z++;
		}
	}
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		words;

	words = ft_count_words(s, c);
	split = (char **)malloc((words + 1) * sizeof(const char *));
	if (!split)
		return (NULL);
	split[words] = NULL;
	split = ft_create_array(split, s, c);
	return (split);
}
