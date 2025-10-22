/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:41:47 by rtiberio          #+#    #+#             */
/*   Updated: 2025/10/22 10:03:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	words_until_pipe(t_token *tokens, int i)
{
	int		p;
	int		w;

	p = 1;
	w = 0;
	while (tokens)
	{
		if (p == i)
		{
			while (tokens && tokens->type != PIPE)
			{
				if (tokens->type == WORD)
					w++;
				tokens = tokens->next;
			}
			break;
		}
		while (tokens && tokens->type != PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
		p++;
	}
	return (w);	
}

void	add_word(t_mini **mini, char *value, int num, t_shell *shell) //terminar o hacer bien
{
	int		i;
	char	*final_value;
	char	*tmp;

	i = 0;
	if (value && value[0])
	{
		tmp = var_exp_check(value, shell);
		final_value = ft_remove_quotes(tmp);
		// (*mini)->cmd_argv[(*mini)->argc] = final_value;
		// (*mini)->argc++;
		// (*mini)->cmd_argv[(*mini)->argc] = NULL;
	}
}

int parser(t_shell *shell, t_token *tokens, t_mini **mini)
{
    t_token *curr;
    t_mini *new_cmd;
	int		num;
	int		i;
  
    curr = tokens;
	i = 0;
    while (curr)
    {
        new_cmd = mini_new();
        if (!new_cmd)
            return (-1);
		i++;
		num = words_until_pipe(tokens, i); // cuento todas las palabras aunque no todas van a ir al char **cmd_argv.
		new_cmd->cmd_argv = malloc(sizeof(char) * (num + 1));
		if (!new_cmd->cmd_argv)
		{
			printf("malloc cmd_argv failed"); // liberar todo lo de t_mini *cmd? o despues cuando salgo de aca?
			return (1);
		}
		while (curr && curr->type != PIPE) // agrego todos los campos hasta encontrar un pipe
        {
            if (curr->type >= IN && curr->type <= APPEND) //(me conviene dividir por el tipo de redir?)
			{
				add_redir(new_cmd, curr, shell);
				curr = curr->next->next;
			}
			else if (curr->type == WORD)
			{
				add_word(new_cmd, curr->value, num, shell); //copio el valor y me fijo si arranca con comillas o no.
            	curr = curr->next;
			}
        }
		if (new_cmd->cmd_argv && new_cmd->cmd_argv[0])
			new_cmd->cmd_name = ft_strdup(new_cmd->cmd_argv[0]);
        mini_lstadd_back(mini, new_cmd);
        if (curr && curr->type == PIPE)
            curr = curr->next;
    }
    // 5. Validate syntax (optional, but recommended)
    // if (syntax_error) return (1);
    return (0);
}
