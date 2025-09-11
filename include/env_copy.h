/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtiberio <rtiberio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:36:53 by rtiberio          #+#    #+#             */
/*   Updated: 2025/08/24 21:57:38 by rtiberio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_COPY_H
# define ENV_COPY_H

# include "minishell_def.h"

//env_copy/env_handling.c
t_env	*create_env_node(char *env_line);
t_env	*create_env(char **env);
t_env	*env_handling(char **env);

//env_copy/env_lists.c
t_env	*env_lstnew(char *name, char *value);
void	env_lstdelone(t_env *lst, void (*del)(void *));
void	env_lstclear(t_env **lst, void (*del)(void *));
t_env	*env_lstlast(t_env *lst);
void	env_lstadd_back(t_env **lst, t_env *new);

//env_copy/env_shlvl.c
void	print_shlvl_warning(int lvl);
t_env	*new_node(const char *name, const char *value);
int		check_lvl_value(char *value);
t_env	*find_node(t_env *env_list, char *str);
bool	update_shlvl(t_env **env_list);

#endif