/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_shell_handling.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:36:53 by rtiberio          #+#    #+#             */
/*   Updated: 2025/10/23 23:07:55 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_COPY_H
# define ENV_COPY_H

# include "minishell_def.h"

//t_shell/env_handling.c
t_env	*create_env_node(char *env_line);
t_env	*create_env(char **env);
t_env	*env_handling(char **env);

//t_shell/env_lists.c
t_env	*env_lstnew(char *name, char *value);
void	env_lstdelone(t_env *lst, void (*del)(void *));
void	env_lstclear(t_env **lst, void (*del)(void *));
t_env	*env_lstlast(t_env *lst);
void	env_lstadd_back(t_env **lst, t_env *new);

//t_shell/env_shlvl.c
void	print_shlvl_warning(int lvl);
t_env	*new_node(const char *name, const char *value);
int		check_lvl_value(char *value);
t_env	*find_node(t_env *env_list, char *str);
bool	update_shlvl(t_env **env_list);

//t_shell/t_shell_handling.c
t_shell	*iniciate_t_shell(char **env);
void	shell_clear(t_shell **shell, void (*del)(void *));
int		check_vars_shell(t_shell *shell, t_token *tokens);
t_env	*new_var(char *src, int i);

//t_shell/env_to_array.c
char	**env_to_array(t_env *env_list);
void	free_env_array(char **envp);
int	count_env_nodes(t_env *env_list);

#endif
