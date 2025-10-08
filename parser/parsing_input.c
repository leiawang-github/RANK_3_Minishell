/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:41:47 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/25 19:11:02 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_mini	*mini_new(void)
{
	t_mini	*new;

	new = malloc(sizeof(t_mini));
	if (!new)
		return (NULL);
    ft_memset(new, 0, sizeof(t_mini));
	return (new);
}

void	mini_lstadd_back(t_mini **lst, t_mini *cmd)
{
	t_mini	*last;

	if (!lst || !cmd)
		return ;
	if (!*lst)
	{
		*lst = cmd;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = cmd;
	cmd->next = NULL;
	return ;
}

void	mini_lstdelone(t_mini *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	else//agregar y corregir para eliminar todos los campos de t_mini
	{
		(*del)(lst->cmd_name);
		(*del)(lst->redirs);
		(*del)(lst->cmd_argv);
		(*del)(lst->assigns);
		(*del)(lst->heredoc_delimiter);
		free(lst);
	}
}

void	mini_lstclear(t_mini **lst, void (*del)(void *))
{
	t_mini	*tmp;

	if (!lst || !*lst || !del)
		return ;
	while (lst && *lst)
	{
		tmp = (*lst)->next;
		mini_lstdelone(*lst, del);
		*lst = tmp;
	}
}

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
		new_cmd->cmd_argv = malloc(sizeof(char*) * (num + 1));
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
				add_word(&new_cmd, curr->value, num, shell); //copio el valor y me fijo si arranca con comillas o no.
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

int	fd_redir(t_token_type redir_type, char *file)
{
	int fd;
	
	fd = -1;
		if (redir_type == IN)
    	fd = open(file, O_RDONLY);
	else if (redir_type == OUT)
    	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir_type == APPEND)
    	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror(file);
	return (fd);
}

char	*ft_clean_quotes(char *dst, char *src)
{
	int		i;
	int		j;
	char	q;

	i = 0;
	j = 0;
	q = 0;
	while (src[i] != '\0')
	{
		if (is_quote(src[i]) == true && q == 0)
			q = src[i];
		else if (q == src[i])
			q = 0;
		else
		{
			dst[j] = src[i];
			j++;
		}
		i++;
	}
	return (dst);
}

int	count_npairs_quotes(char *tmp)
{
	int		i;
	int		n_pairs;
	char	q;

	i = 0;
	n_pairs = 0;
	q = 0;
	while (tmp[i] != '\0')
	{
		if (is_quote(tmp[i]) == true)
		{
			if (q == 0)
				q = tmp[i];
			else if (tmp[i] == q)
			{
				q = 0;
				n_pairs++;
			}
		}
		i++;
	}
	return (n_pairs);
}

char	*ft_remove_quotes(char *tmp)
{
	int		len;
	char	*clean;
	int		n_pairs;

	if (!tmp)
		return (NULL);
	n_pairs = count_npairs_quotes(tmp);
	if (n_pairs == 0)
		clean = ft_strdup(tmp);
	else
	{
		len = ft_strlen(tmp) - (2 * n_pairs);
		clean = malloc(sizeof(char) * (len + 1));
		if (!clean)
			return (NULL);
		clean [len] = '\0';
		ft_clean_quotes(clean, tmp);
	}
	return (clean);
}

t_redir	*create_redir(t_token *curr, t_shell *shell)
{
	t_redir	*new_redir;
    char    *tmp;

    tmp = NULL;
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);//ver bien que devolver
	ft_memset(new_redir, 0, sizeof(t_redir));
	new_redir->redir_type = curr->type;
	if (curr->next && curr->next->value)
	{
		if (new_redir->redir_type == HEREDOC)
		{
			new_redir->do_expand = 1; // TODO: implement check_if_expands function
			new_redir->delimiter = ft_remove_quotes(curr->next->value);
			new_redir->fd = -1;
		}
		else
		{
            tmp = var_exp_check(curr->next->value, shell);
			new_redir->file = ft_remove_quotes(tmp); //ver si tiene comillas y sacarlas
			new_redir->fd = fd_redir(new_redir->redir_type, new_redir->file);
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
		return ;//ver bien que devolver
	add_back_redir(&new_cmd->redirs, new_redir);
	return ;
}

void	add_word(t_mini **mini, char *value, int num, t_shell *shell) //terminar o hacer bien
{
	(void)mini; // 标记未使用的参数
	(void)num;  // 标记未使用的参数
	(void)value; // 标记未使用的参数
	(void)shell; // 标记未使用的参数
	// TODO: implement add_word function
}
