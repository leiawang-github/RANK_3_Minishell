/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 18:00:41 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 11:09:22 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = (char *)malloc(ft_strlen(s1) + 1);
	if (tmp == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		tmp[i] = s1[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

// int main(void)
// {
// 	char *tmp;
//     char s1[] = "hello from another world";

//     // 调用 ft_strdup 复制字符串 s1
//     tmp = ft_strdup(s1);
//     if (tmp == NULL)
//     {
//         printf("Memory allocation failed\n");
//         return (1);
//     }

//     // 打印复制后的字符串内容
//     printf("Strdup original output: %s\n", s1);
//     printf("Ft_strdup output: %s\n", tmp);

//     // 释放内存
//     free(tmp);

//     return (0);
// }