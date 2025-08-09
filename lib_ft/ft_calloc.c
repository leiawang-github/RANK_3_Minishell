/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 15:23:32 by leiwang           #+#    #+#             */
/*   Updated: 2024/07/29 14:20:48 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*arr;
	size_t			i;

	i = 0;
	arr = malloc(size * count);
	if (arr == NULL)
		return (NULL);
	while (i < size * count)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}

// int main(void)
// {
// 	char arr[] = "hey you this is a messege from AD";

// 	void *ptr1, *ptr2;

//     // 调用 ft_calloc，并存储返回值
//     ptr1 = ft_calloc(5, 5);
//     if (ptr1 == NULL)
//     {
//         printf("ft_calloc failed\n");
//         return (1);
//     }

//     // 调用标准 calloc，并存储返回值
//     ptr2 = calloc(5, 5);
//     if (ptr2 == NULL)
//     {
//         printf("calloc failed\n");
//         return (1);
//     }

//     // 打印两个指针的值（地址）
//     printf("Original calloc output: %p\n", ptr2);
//     printf("Ft_calloc output: %p\n", ptr1);

//     // 释放动态分配的内存
//     free(ptr1);
//     free(ptr2);

//     return (0);

// }