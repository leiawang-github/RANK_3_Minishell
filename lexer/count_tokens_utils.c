/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:16:02 by codespace         #+#    #+#             */
/*   Updated: 2025/08/30 17:27:11 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_delim(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (true);
	return (false);
}

bool	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (true);
	return (false);
}

int	update_i_quote(char *input, int i)
{
	char	quote;

	quote = input[i];
	i++;
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
	{
		printf("Syntax error open quote: '%c'\n", quote);
		return (-1);
	}
	i++;
	return (i);
}

int	update_i_delim(char *input, int i)
{
	if (!input[i])
		return (i);
	if (!input[i + 1])
		return (i + 1);
	if ((input[i] == '<' && input [i + 1] == '<')
		|| (input[i] == '>' && input [i + 1] == '>'))
		return (i + 2);
	if ((input[i] == '|' && input [i + 1] == '|'))
	{
		printf("Syntax error double pipe not handled: '||'\n");
		return (-2);
	}
	return (i + 1);
}

int	update_i_word(char *input, int i)
{
	while (input[i] != '\0' && ft_isspace(input[i]) == 0
		&& is_delim(input[i]) == false)
		i++;
	return (i);
}
