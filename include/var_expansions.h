/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansions.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:36:53 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/25 19:05:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAR_EXPANSIONS_H
# define VAR_EXPANSIONS_H

# include "minishell_def.h"

//variable_expansion_utils.c
void	*free_t_varexp(t_varexp *var);
char	*exp_alpha_only(char *str, t_shell *shell);
char	*create_exp_sub(char *src, int end, int start, t_shell *shell);
char	*non_expandable_sub(char *src, int *i);

// variable_expansion_check.c
int     i_update_exp(char *src, int i);
int     last_i_single_quote(char *src, int i);
int     exp_check(char *src);

//variable_expansion.c
int     create_var_exp(char *src, t_varexp *var, t_shell *shell);
int	    caculate_new_len(t_varexp *var);
void	fill_new_str(t_varexp *var);
int	    build_varexp(char *src, t_varexp **var, t_shell *shell);
char	*var_exp_check(char	*src, t_shell *shell);

#endif