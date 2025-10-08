/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:18:18 by codespace         #+#    #+#             */
/*   Updated: 2025/09/25 18:58:31 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_var_exp(char *src, t_varexp *var, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i] != '\0' && j < var->num_subs)
	{
		var->i_src[j] = i;
		if (src[i] == '\'')
		{
			i = last_i_single_quote(src, i);
			var->subs[j] = ft_substr(src, var->i_src[j], i - var->i_src[j]);
		}
		else if (src[i] == '$')
		{
			i = i_update_exp(src, i);
			var->subs[j] = create_exp_sub(src, i, var->i_src[j], shell);
		}
		else
			var->subs[j] = non_expandable_sub(src, &i);
		if (var->subs[j] == NULL)
			return (1);
		j++;
	}
	return (0);
}

int	caculate_new_len(t_varexp *var)
{
	int	i;
	int	j;
	int len;
	
	i = 0;
	len = 0;
	while (var->subs[i] != NULL)
	{
		j = 0;
		while (var->subs[i][j] != '\0')
		{
			len++;
			j++;
		}
		i++;
	}
	return (len);
}

void	fill_new_str(t_varexp *var)
{
	int	i;
	int	j;
	int	z;
	
	i = 0;
	z = 0;
	while (var->subs[i] != NULL)
	{
		j = 0;
		while (var->subs[i][j] != '\0')
		{
			var->new_str[z] = var->subs[i][j];
			j++;
			z++;
		}
		i++;
	}
}

int	build_varexp(char *src, t_varexp **var, t_shell *shell)
{
	int r;
	
	r = create_var_exp(src, *var, shell);
	if (r == 1)
		return (1);
	(*var)->len_new_str = caculate_new_len(*var);
	(*var)->new_str = malloc(sizeof(char) * ((*var)->len_new_str + 1));
	if (!(*var)->new_str)
		return (1);
	(*var)->new_str[(*var)->len_new_str] = '\0';
	fill_new_str(*var);
	return (0);
}

char	*var_exp_check(char	*src, t_shell *shell)
{
	t_varexp	*tmp;
	char		*result;
	int			r;

	if (!src)
		return (NULL);
	tmp = malloc(sizeof(t_varexp));
	if (!tmp)
		return (NULL);
	ft_memset(tmp, 0, sizeof(t_varexp));
	tmp->num_subs = exp_check(src);
	tmp->subs = malloc(sizeof(char *) * (tmp->num_subs + 1));
	if (!tmp->subs)
		return (free_t_varexp(tmp));
	tmp->i_src = malloc(sizeof(int) * (tmp->num_subs + 1));
	if (!tmp->i_src)
		return (free_t_varexp(tmp));
	r = build_varexp(src, &tmp, shell);
	if (r == 1)
		return (free_t_varexp(tmp));
	result = ft_strdup(tmp->new_str);
	free_t_varexp(tmp);
	if (!result)
		return (NULL);
	return (result);
}
