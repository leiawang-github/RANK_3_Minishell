/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:36:53 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/25 19:20:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell_def.h"

//parsing_input.c
t_mini	*mini_new(void);
void	mini_lstadd_back(t_mini **lst, t_mini *cmd);
void	mini_lstdelone(t_mini *lst, void (*del)(void *));
void	mini_lstclear(t_mini **lst, void (*del)(void *));
int	words_until_pipe(t_token *tokens, int i);
int parser(t_shell *shell, t_token *tokens, t_mini **mini);
int	fd_redir(t_token_type redir_type, char *file);
char	*ft_clean_quotes(char *dst, char *src);
int	count_npairs_quotes(char *tmp);
char	*ft_remove_quotes(char *tmp);
t_redir	*create_redir(t_token *curr, t_shell *shell);
void	add_back_redir(t_redir **lst_redir, t_redir *new_redir);
void	add_redir(t_mini *new_cmd, t_token *curr, t_shell *shell);
void	add_word(t_mini **mini, char *value, int num, t_shell *shell);

#endif