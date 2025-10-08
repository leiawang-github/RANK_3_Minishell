/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 18:47:29 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/26 12:39:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_def.h"
# include "t_shell_handling.h"
# include "lexer.h"
# include "parsing.h"
# include "var_expansions.h"

//exit_functions/free_mem.c
void	*free_array(char **array);
void	free_null(void *ptr);

//utils/array_size.c
int		array_size(char **array);

//utils/ft_strcmp.c
int		ft_strcmp(const char *s1, const char *s2);

//utils/ft_isspace.c
int		ft_isspace(int c);

//main.c
int		status_code(int arg);
int		check_input_start(char *input);
void	error_input(char *input, char *msg);
int		handle_input(t_shell *shell, t_token **tokens);

#endif
