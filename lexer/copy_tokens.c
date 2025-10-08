/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 09:04:43 by codespace         #+#    #+#             */
/*   Updated: 2025/08/30 19:03:56 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_token_quotes(char **tokens, int z, char *s, int i)
{
	int		len;
	int		start;

	start = i;
	i = check_consecutive_quotes(s, i);
	len = i - start;
	tokens[z] = malloc(sizeof(char) * (len + 1));
	if (!tokens[z])
		return (-1);
	ft_strlcpy(tokens[z], s + start, len + 1);
	return (i);
}

int	copy_token_ndelim(char **tokens, int z, char *s, int i)
{
	char	d;
	int		len;

	d = s[i];
	len = 1;
	if (s[i + 1] != '\0' && s[i + 1] == d)
		len = 2;
	tokens[z] = malloc(sizeof(char) * (len + 1));
	if (!tokens[z])
		return (-1);
	tokens[z][len] = '\0';
	tokens[z][0] = s[i];
	i++;
	if (len == 2)
	{
		tokens[z][1] = s[i];
		i++;
	}
	return (i);
}

int	copy_token_words(char **tokens, int z, char *s, int i)
{
	int	start;
	int	len;

	start = i;
	while (s[i] && ft_isspace(s[i]) == 0)
		i++;
	len = i - start;
	tokens[z] = malloc(sizeof(char) * (len + 1));
	if (!tokens[z])
		return (-1);
	ft_strlcpy(tokens[z], s + start, len + 1);
	return (i);
}
