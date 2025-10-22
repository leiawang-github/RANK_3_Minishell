/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:17:56 by leiwang           #+#    #+#             */
/*   Updated: 2025/10/22 17:51:58 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

char	*expand_vars(char *line)
{
	char	*res;
	int		i;
	int		len;
	char	*var;
	char	*val;
	char	buf[4096];

	i = 0;
	buf[0] = '\0';
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
		{
			i++;
			len = var_len(&line[i]);
			var = ft_strndup(&line[i], len);
			val = getenv(var);
			free(var);
			if (val)
				ft_strlcat(buf, val, sizeof(buf));
			i += len;
		}
		else
			strlcat(buf, (char[]){line[i++], 0}, sizeof(buf));
	}
	res = ft_strdup(buf);
	free(line);
	return (res);
}

static int	var_len(char *s)
{
	int	i;

	i = 0;
	while (s[i] && (ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
		i++;
	return (i);
}
