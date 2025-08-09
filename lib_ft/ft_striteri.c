/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:56:38 by leia              #+#    #+#             */
/*   Updated: 2024/07/29 12:16:09 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;
	int	s_len;

	i = 0;
	s_len = ft_strlen(s);
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

// void ft_toupper2(unsigned int index, char *c) {
//     if ('a' <= *c && *c <= 'z') {
//         *c = *c - 32;  // 转换为大写
//     }
// }
// int main(void) {
//     char s[] = "hello world";
//     // 调用 ft_striteri 函数
//     ft_striteri(s, ft_toupper2);
//     // 打印结果
//     printf("Output: %s\n", s);  // 期望输出 "HELLO WORLD"
//     return (0);
// }

// Mantal Version
// void	ft_striter(char *s, void (*f)(char *))
// {
// 	while (*s)
// 	{
// 		f(&*s);
// 		s++;
// 	}
// }
