/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:19:51 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/30 21:09:27 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_input_start(char *input)
{
	if (input[0] == '|' && input[1] == '|')
	{
		printf("syntax error near unexpected token `||'\n");
		return (1);
	}
	else if (input[0] == '|')
	{
		printf("syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

int	check_input_end(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
		i++;
	i--;
	if (i >= 0)
	{
		if (input[i] == '>' || input[i] == '<')
		{
			printf("syntax error near unexpected token `newline'\n");
			return (1);
		}
		else if (input[i] == '|')
		{
			printf("syntax error near unexpected token `|'\n");
			return (1);
		}
	}
	return (0);
}

int	check_input_quotes(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (is_quote(input[i]) == true)
			i = update_i_quote(input, i);
		else
			i++;
		if (i == -1)
			return (1);
	}
	return (0);
}

void	print_syntax_error_type(t_token_type type)
{
	if (type == IN)
		printf("syntax error near unexpected token `%s'\n", "<");
	else if (type == OUT)
		printf("syntax error near unexpected token `%s'\n", ">");
	else if (type == HEREDOC)
		printf("syntax error near unexpected token `%s'\n", "<<");
	else if (type == APPEND)
		printf("syntax error near unexpected token `%s'\n", ">>");
	else if (type == PIPE)
		printf("syntax error near unexpected token `%s'\n", "|");
	return ;
}

int	has_consecutive_operators(t_token *data)
{
	while (data && data->next != NULL)
	{
		if (data->type >= IN && data->type <= APPEND)
		{
			if (data->next->type >= IN && data->next->type <= PIPE)
			{
				print_syntax_error_type(data->next->type);
				status_code(2);
				return (1);
			}
		}
		else if (data->type == PIPE && data->type == PIPE)
		{
			print_syntax_error_type(data->next->type);
			status_code(2);
			return (1);
		}
		data = data->next;
	}
	status_code(0);
	return (0);
}
