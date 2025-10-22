/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_shell_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 00:30:37 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/26 12:54:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	shell_clear(t_shell **shell, void (*del)(void *))
{
	if ((*shell)->env_list)
		env_lstclear(&(*shell)->env_list, &free_null);
	if ((*shell)->vars)
		env_lstclear(&(*shell)->vars, &free_null);
	free(*shell);
	*shell = NULL;
}

t_env	*new_var(char *src, int i)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	ft_memset(new, 0, sizeof(t_env));
	if (!src)
		return (new);
	else
	{
		new->name = ft_substr(src, 0, i);
		if (!new->name)
		{
			free(new);
			return (NULL);
		}
		new->value = ft_substr(src, i + 1, ft_strlen(src) - i - 1);
		if (!new->value)
		{
			free(new->name);
			free(new);
			return (NULL);
		}
		return (new);
	}
}

int	check_vars_shell(t_shell *shell, t_token *tokens)
{
	t_env	*var_tmp;
	t_token	*tmp;
	int		i;

	tmp = tokens;
	var_tmp = NULL;
	i = 0;
	if (tmp->type == WORD && tmp->next == NULL)
	{
		if (isalpha(tmp->value[i]) || tmp->value[i] == '_')
		{
			while (isalnum(tmp->value[i]) || tmp->value[i] == '_')
				i++;
			if (i != 0 && tmp->value[i] == '=' && tmp->value[i + 1] != '\0')
			{
				var_tmp = new_var(tmp->value, i);
				if (!var_tmp)
					return (-1);
				env_lstadd_back(&shell->vars, var_tmp);
				return (1);
			}
		}
		
	}
	return (0);
}

t_shell	*iniciate_t_shell(char **env)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	ft_memset(shell, 0, sizeof(t_shell));
	shell->env_list = env_handling(env);
	if (!shell->env_list)
	{
		shell_clear(&shell, &free_null);
		return (NULL);
	}
	return (shell);
}
