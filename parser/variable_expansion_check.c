/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_check.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:18:18 by codespace         #+#    #+#             */
/*   Updated: 2025/09/25 18:55:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	i_update_exp(char *src, int i)
{
	i++;
	if (src[i] == '\0')
		return (i);
	else if (src[i] == '?')
		i++;
	else if (ft_isalpha(src[i]) || src[i] == '_')
	{
		i++;
		while (ft_isalnum(src[i]) || src[i] == '_')
			i++;
	}
	return (i);
}

int	last_i_single_quote(char *src, int i)
{
	i++;
	while (src[i] != '\0' && src[i] != '\'')
		i++;
	if (src[i] == '\'')
		i++;
	return (i);
}

int	exp_check(char *src)
{
	int		i;
	int		n;

	i = 0;
	n = 1;
	while (src[i] != '\0')
	{
		if (src[i] == '\'')
			i = last_i_single_quote(src, i);
		if (src[i] == '$')
		{
			n++;
			i = i_update_exp(src, i);
		}
		else
			i++;
	}
	return (n);
}
