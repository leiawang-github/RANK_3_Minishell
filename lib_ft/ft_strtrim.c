/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:41:03 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 13:38:48 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		i;
	char	*result;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (s1[start] != '\0' && ft_strchr(set, s1[start]))
		start++;
	while (end >= start && ft_strchr(set, s1[end]))
		end--;
	result = (char *)malloc((end - start + 2) * sizeof(char));
	if (!result)
		return (NULL);
	ft_memcpy(result, &s1[start], end - start + 1);
	result[end - start + 1] = '\0';
	return (result);
}

// int main(void)
// {
//     char const s1[] = "Hello World";
//     char const set[] = "Hesdl";

//     char *result = ft_strtrim(s1, set);
//     if (result) {
//         printf("Output should be: '%s'\n", result);
//         free(result);  // 记得释放内存
//     } else {
//         printf("An error occurred.\n");
//     }

//     return (0);
// }