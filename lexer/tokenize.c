/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 09:48:29 by codespace         #+#    #+#             */
/*   Updated: 2025/09/02 15:58:37 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	type_token(char *token)
{
	if (token == NULL)
		return (END);
	else if (ft_strcmp(token, "<") == 0)
		return (IN);
	else if (ft_strcmp(token, ">") == 0)
		return (OUT);
	else if (ft_strcmp(token, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(token, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(token, "|") == 0)
		return (PIPE);
	return (WORD);
}

char	**fill_tokens(char **tokens, char *s, int num_tokens)
{
	int	i;
	int	z;

	i = 0;
	z = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]) == 1)
			i++;
		if (!s[i])
			break ;
		if (s[i] == '\'' || s[i] == '"')
			i = copy_token_quotes(tokens, z, s, i);
		else if (is_delim(s[i]))
			i = copy_token_ndelim(tokens, z, s, i);
		else
			i = copy_token_words(tokens, z, s, i);
		if (i < 0)
			return (free_array(tokens));
		z++;
		if (z >= num_tokens)
			break ;
	}
	return (tokens);
}

char	**tokenize(char *input, int num_tokens)
{
	char	**tokens;

	tokens = malloc(sizeof(char *) * (num_tokens + 1));
	if (!tokens)
		return (NULL);
	tokens[num_tokens] = NULL;
	fill_tokens(tokens, input, num_tokens);
	return (tokens);
}
