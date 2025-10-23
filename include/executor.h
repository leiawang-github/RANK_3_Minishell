/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:08:08 by leiwang           #+#    #+#             */
/*   Updated: 2025/10/24 01:46:55 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
#define EXECUTOR_H

/* Standard headers */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell_def.h"

/* Global last status (provided by shell core) */
extern int g_last_status;

int exec_single_builtin_parent(t_mini *cmd, t_env *env_list);
int exec_pipeline(t_mini *cmd, t_shell *shell);

/* Heredoc preprocess */
int prepare_all_heredocs(t_mini *head);

/* Error helper */
void error_status(enum e_err kind);
int ft_errno(const char *file, int saved_errno, enum e_err kind);
int err_msg(const char *prefix, const char *suffix, enum e_err kind);

//implementation of builtins
int	is_child_builtin(t_mini *cmd);
int mini_echo(char **argv);
int mini_cd(char **argv, t_env *env);
int mini_pwd(void);
int	mini_unset(char **argv, t_env **envp);
int mini_export(char **argv, t_env **env_list);
int mini_env(char **argv, t_env *env);
int mini_exit(char **argv);

void	set_parent_ignore_signals(void (**old_i)(int), void (**old_q)(int));
int	    fail_wait_cleanup(int *pfd, void (*old_i)(int), void (*old_q)(int));
void	restore_parent_signals(void (*old_i)(int), void (*old_q)(int));
void	setup_child_signals(void);

int	apply_all_redirs(t_redir *redirs);
int exec_single_builtin_parent(t_mini *cmd, t_env *env_list);
int	run_builtin_in_parent(char **argv, t_env *env_list);
int	restore_parent_fds(int parent_fds[3]);

int	exec_one_child(t_mini *cmd, t_shell *shell);
int	exec_all_children(t_mini *node, t_shell *sh);
void	run_external(char **argv, char **envp);


#endif
