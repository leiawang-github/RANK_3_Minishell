/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redir_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:41:47 by rtiberio          #+#    #+#             */
/*   Updated: 2025/10/22 09:52:26 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	fd_redir(t_token_type redir_type, char *file)
// {
// 	int fd;
	
// 	fd = -1;
// 		if (redir_type == IN)
//     	fd = open(file, O_RDONLY);
// 	else if (redir_type == OUT)
//     	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	else if (redir_type == APPEND)
//     	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd == -1)
// 		perror(file);
// 	return (fd);
// }

bool	check_if_heredoc_expands(char *delimiter) // heredoc only expands if it has no quotes
{
	int		i;

	i = 0;
	while (delimiter[i] != '\0')
	{
		if (is_quote(delimiter[i]) == true)
			return (false); // no expande si tiene comillas
		i++;
	}
	return (true); // expande si no tiene comillas
}

t_redir_type	assign_redir_type(t_token_type token_type)
{
	if (token_type == IN)
		return (IN);
	else if (token_type == OUT)
		return (OUT);
	else if (token_type == HEREDOC)
		return (HEREDOC);
	else if (token_type == APPEND)
		return (APP);
	return (-1); // error
}

t_redir	*create_redir(t_token *curr, t_shell *shell)
{
	t_redir	*new_redir;
    char    *tmp;

    tmp = NULL;
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);//TODO chequear bien que devolver
	ft_memset(new_redir, 0, sizeof(t_redir));
	new_redir->redir_type = assign_redir_type(curr->type);
	if (curr->next && curr->next->value)
	{
		if (new_redir->redir_type == HEREDOC)
		{
			new_redir->hdoc_expand = check_if_heredoc_expands(curr->next->value); //si tiene quotes no expande
			new_redir->delimiter = ft_remove_quotes(curr->next->value);
			new_redir->heredoc_fd = -1;
		}
		else
		{
            tmp = var_exp_check(curr->next->value, shell); // chequear si aplica la expansion y hacerla
			new_redir->target = ft_remove_quotes(tmp); //ver si tiene comillas y sacarlas
		}
	}
	return (new_redir);
}

void	add_back_redir(t_redir **lst_redir, t_redir *new_redir)
{
	t_redir	*last;

	if (!lst_redir || !new_redir)
		return ;
	if (!*lst_redir)
	{
		*lst_redir = new_redir;
		return ;
	}
	last = *lst_redir;
	while (last->next)
		last = last->next;
	last->next = new_redir;
	new_redir->next = NULL;
	return ;
}

void	add_redir(t_mini *new_cmd, t_token *curr, t_shell *shell)
{
	t_redir	*new_redir;
	
	new_redir = create_redir(curr, shell);
	if (!new_redir)
		return ;//TODO: chequear bien que devolver
	add_back_redir(&new_cmd->redirs, new_redir);
	return ;
}
