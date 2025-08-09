/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 16:39:43 by leiwang           #+#    #+#             */
/*   Updated: 2024/08/12 15:06:39 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_s;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	new_s = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new_s == NULL)
		return (NULL);
	while (s1[j] != '\0')
	{
		new_s[i + j] = s1[j];
		j++;
	}
	while (s2[k] != '\0')
	{
		new_s[i + j + k] = s2[k];
		k++;
	}
	new_s[i + j + k] = '\0';
	return (new_s);
}

// int	main(void)
// {
// 	char const *s1 = "why not";
// 	char const *s2 = "code\n";

// 	char *result = ft_strjoin(s1, s2);

// 	if (result != NULL)
// 	{
// 		printf("Output: %s", result);
// 		free(result);
// 	}
// 	else
// 	{
// 		printf("Memory allocation failed\n");
// 	}

// 	return (0);
// }