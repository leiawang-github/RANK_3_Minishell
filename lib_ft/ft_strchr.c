/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:14:33 by leiwang           #+#    #+#             */
/*   Updated: 2024/09/02 19:32:52 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
Man: The strchr() function locates the first occurrence of c (converted to a char) in the string pointed to by s. 
The terminating null character is considered to be part of the string; therefore if c is `\0', the functions locate 
the terminating `\0'.

Return Value: The functions strchr() returns a pointer to the located character, or NULL if the character does not 
appear in thestring.

*/

// Approach one: use index
// char	*ft_strchr(const char *s, int c)
// {
// 	size_t	i;
// 	char	*tmp;

// 	i = 0;
// 	tmp = (char *)s;
// 	while (tmp[i] != '\0')
// 	{
// 		if (tmp[i] == (char)c)
// 			return (&tmp[i]);//This is how we fetch a returning value of a char: by getting char's address
// 		i++;
// 	}
// 	return (NULL);
// }

//Approach two:from Mantal github, use pointer
char *ft_strchr(const char *s, int c)
{
	while(*s && *s != c)
		s++;
	return (*s == c ? (char *)s : NULL);
}

int main(void)
{
	char *str= "What a beautiful day!";
	char *substr1 = ft_strchr(str, 'W');
	char *substr2 = strchr(str, 'W');

	printf("This will give us a char: %c\n", *substr1);
	printf("This will give us a char: %c\n", *substr2);
	
	printf("Aha, can we know the position of %c in str?!: %ld\n", *substr1, substr1 - str);
	printf("Aha, can we know the position of %c in str?!: %ld\n", *substr2, substr2 - str);

	printf("This will give us a str: %s\n", substr1);
	printf("This will give us a str: %s\n", substr2);

	char *substr3 = ft_strchr(str, '\n');
	char *substr4 = strchr(str, '\n');

	printf("Let's test \\n as para case: %s\n", substr3);
	printf("Let's test \\n as para case: %s\n", substr4);
}


