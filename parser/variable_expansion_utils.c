/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion _utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:18:18 by codespace         #+#    #+#             */
/*   Updated: 2025/09/25 19:00:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <ctype.h>

void	*free_t_varexp(t_varexp *var)
{
	int	i;

	if (!var)
		return (NULL);
	if (var->subs)
	{
		i = 0;
		while (i < var->num_subs)
		{
			if (var->subs[i])
				free(var->subs[i]);
			i++;
		}
		free(var->subs);
	}
	if (var->i_src)
		free(var->i_src);
	if (var->new_str)
		free(var->new_str);
	free(var);
	return (NULL);
}

char	*exp_alpha_only(char *str, t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->env_list;
	while (tmp)
	{
		if (ft_strcmp(str, tmp->name) == 0)
			return(ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	tmp = shell->vars;
	while (tmp)
	{
		if (ft_strcmp(str, tmp->name) == 0)
			return(ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (strdup(""));
}

char	*create_exp_sub(char *src, int end, int start, t_shell *shell)
{
	char	*sub_str;
	char	*value;

	sub_str = ft_substr(src, start + 1, end - (start + 1));
	if (!sub_str)
		return (NULL);
	if (ft_strcmp(sub_str, "?") == 0)
		value = ft_itoa(status_code(-1));
	else if (ft_strcmp(sub_str, "$") == 0 || ft_strcmp(sub_str, "-") == 0
		|| ft_strcmp(sub_str, "_") == 0 || isdigit(sub_str[0]) == true)
		value = ft_strdup("");
	else
		value = exp_alpha_only(sub_str, shell);
	free (sub_str);
	if (!value)
		return (NULL);
	return (value);
}

char	*non_expandable_sub(char *src, int *i)
{
	int		start;
	int		end;
	char	*value;

	start = *i;
	while (src[*i] != '\0')
	{
		if (src[*i] == '\'' || src[*i] == '$')
			break ;
		(*i)++;
	}
	end = *i;
	if (end > start && (src[end] == '\'' || src[end] == '$'))
		end--;
	value = ft_substr(src, start, end - start);
	if (!value)
		return (NULL);
	return (value);
}
