/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 22:08:57 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/30 19:04:33 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*token_list(char **tokens, int num_tokens)
{
	int		i;
	t_token	*data;
	t_token	*new;

	i = 0;
	data = NULL;
	while (i < num_tokens)
	{
		new = token_node_create(tokens[i]);
		if (!new)
		{
			token_lstclear(&data, &free_null);
			return (NULL);
		}
		token_lstadd_back(&data, new);
		i++;
	}
	if (has_consecutive_operators(data) == 1)
	{
		token_lstclear(&data, &free_null);
		return (NULL);
	}
	return (data);
}

int	check_consecutive_quotes(char *input, int i)
{
	i = update_i_quote(input, i);
	while (input[i] != '\0' && ft_isspace(input[i]) == 0)
	{
		if (is_quote(input[i]) == true)
			i = update_i_quote(input, i);
		else
			i++;
	}
	return (i);
}

int	count_tokens(char *input, int num_tokens)
{
	int		i;

	i = 0;
	while (input[i] != '\0')
	{
		while (input[i] && ft_isspace(input[i]) == 1)
			i++;
		if (!input[i])
			break ;
		else if (is_quote(input[i]) == true)
			i = check_consecutive_quotes(input, i);
		else if (is_delim(input[i]) == true)
			i = update_i_delim(input, i);
		else
			i = update_i_word(input, i);
		if (i < 0)
			return (i);
		num_tokens++;
	}
	return (num_tokens);
}

int	precheck_input(char *input)
{
	if (!input || !*input)
	{
		status_code(0);
		return (0);
	}
	if (check_input_start(input) == 1)
	{
		status_code(2);
		return (1);
	}
	if (check_input_end(input) == 1)
	{
		status_code(2);
		return (1);
	}
	if (check_input_quotes(input) == 1)
	{
		status_code(2);
		return (1);
	}
	status_code(0);
	return (0);
}

t_token	*lexer(char *input)
{
	char	**tokens;
	int		num_tokens;
	t_token	*data;

	num_tokens = 0;
	if (precheck_input(input) == 1)
		return (NULL);
	num_tokens = count_tokens(input, num_tokens);
	if (num_tokens < 0)
		return (NULL);
	tokens = tokenize(input, num_tokens);
	if (!tokens)
		return (NULL);
	data = token_list(tokens, num_tokens);
	free_array(tokens);
	if (!data)
		return (NULL);
	status_code(0);
	return (data);
}
// TODO: LO QUE SIGUE ES PARTE DEL PARSING Y DE CHEQUEAR SI SON VALIDOS LOS TOKENS.

//?Asi quedaria si esta sin comentarios y sin prinft de chequeo (20 lineas)!
// t_token	*lexer(char *input)
// {
// 	char	**tokens;
// 	int		num_tokens;
// 	t_token	*data;

// 	if (precheck_input(input) == 1)
// 		return (NULL);
// 	num_tokens = count_tokens(input);
// 	if (num_tokens < 0)
// 		return (NULL);
// 	tokens = tokenize(input, num_tokens);
// 	if (!tokens)
// 		return (NULL);
// 	data = token_list(tokens, num_tokens);
// 	if (!data)
// 	{
// 		free_array(tokens);
// 		return (NULL);
// 	}
// 	free_array(tokens);
// 	return (data);
// }
