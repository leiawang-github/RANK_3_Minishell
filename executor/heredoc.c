/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pre.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:09 by leia              #+#    #+#             */
/*   Updated: 2025/08/31 22:04:24 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	heredoc_readline(const char *delimiter, int do_expand)
{
    char	*line;
    int		should_expand;

    while (1)
    {
        line = readline("> ");
        if (!line) // EOF
            break ;
        if (ft_strcmp(line, delimiter) == 0) // hit delimiter
        {
            free(line);
            break ;
        }
        should_expand = do_expand && (ft_strchr(line, '$') != NULL);
        if (should_expand)
        {
            char *expanded = expand_variables(line);
            if (!expanded)
            {
                free(line);
                return -1; // memory allocation failure during expansion
            }
            printf("%s\n", expanded);
            free(expanded);
        }
        else
            printf("%s\n", line);
        free(line);
    }
    return 0;
}