/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:36:53 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/25 19:29:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell_def.h"

//lexer/copy_tokens.c
int				copy_token_quotes(char **tokens, int z, char *s, int i);
int				copy_token_ndelim(char **tokens, int z, char *s, int i);
int				copy_token_words(char **tokens, int z, char *s, int i);

//lexer/count_tokens_utils.c
bool			is_delim(char c);
bool			is_quote(char c);
int				update_i_quote(char *input, int i);
int				update_i_delim(char *input, int i);
int				update_i_word(char *input, int i);

//lexer/lexer_input.c
t_token			*token_list(char **tokens, int num_tokens);
int	            check_consecutive_quotes(char *input, int i);
int				count_tokens(char *input, int num_tokens);
int				precheck_input(char *input);
t_token			*lexer(char *input);

//lexer/syntax_check.c
int				check_input_start(char *input);
int				check_input_end(char *input);
int             check_input_quotes(char *input);
void			print_syntax_error_type(t_token_type type);
int				has_consecutive_operators(t_token *data);

//lexer/t_token_edit_lst.c
void			token_lstadd_back(t_token **lst, t_token *new);
void			token_lstdelone(t_token *lst, void (*del)(void *));
void			token_lstclear(t_token **lst, void (*del)(void *));
t_token			*token_lstnew(char *value, t_token_type type);
t_token			*token_node_create(char *token);

//lexer/tokenize.c
t_token_type    type_token(char *token);
char			**fill_tokens(char **tokens, char *s, int num_tokens);
char			**tokenize(char *input, int num_tokens);

#endif