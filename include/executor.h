/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:08:08 by leiwang           #+#    #+#             */
/*   Updated: 2025/10/19 18:44:05 by leiwang          ###   ########.fr       */
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

/* Readline (used by heredoc) */
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell_def.h"

/* Forward decls */
typedef struct s_mini   t_mini;

/* Command classification */
typedef enum e_cmd_type
{
    CMD_REDIR_ONLY,  /* empty command with only redirections */
    CMD_BUILTIN,
    CMD_EXTERNAL,
    CMD_INVALID
} t_cmd_type;

enum e_err
{
    ERR_NONE,
    ERR_SYNTAX,
    ERR_CMD_NOT_FOUND,
    ERR_CANNOT_EXEC,     // perm denied / is a directory / exec format
    ERR_SYS_BUILTIN,     // 内建的系统错误
    ERR_REDIR,           // 打开重定向失败
    ERR_SIGINT, ERR_SIGQUIT
};

// Redirection types are now defined in minishell_def.h as t_redir_type

/* Global last status (provided by shell core) */
extern int g_last_status;

/* Analyzer */
t_cmd_type analyze_cmd(t_mini *cmd);


/* Pipeline dispatcher */
int execute_command(t_mini *pipeline, char **envp, t_env *env_list);

/* Single/pipeline executors */
int exec_redirs(t_redir *redirs);
int probe_redirs(t_redir *redirs);
int exec_builtin_in_single_cmd(t_mini *pipeline, t_env *env_list);
int exec_single_external(t_mini *pipeline, t_env *env_list, char **envp);
int exec_pipeline(t_mini *head, char **envp, t_env *env_list);

/* Path resolution */
char *which_path(char *cmd_name, t_env *env_list);

/* Heredoc preprocess */
int preprocess_heredoc(t_mini *head);

/* Error helper */
void error_status(enum e_err kind);
int ft_errno(const char *file, int saved_errno, enum e_err kind);
//int err_msg(const char *msg, enum e_err kind);
int err_msg(const char *prefix, const char *suffix, enum e_err kind);

//implementation of builtins
int builtin_echo(char **argv);
int builtin_cd(char **argv, t_env *env);
int builtin_pwd(void);
int builtin_export(char **argv, t_env **env_list);
int builtin_unset(char **argv, t_env **env_list);
int builtin_env(char **argv, t_env *env_list);
int builtin_exit(char **argv);

/* Signal handlers */
void sigint_handler(int sig);
void sigquit_handler(int sig);
void setup_signal_handlers(void);
void restore_signal_handlers(void);
int install_heredoc_parent_signals(void);
int restore_interactive_signals(void);



int is_valid_var_name(const char *name);


#endif
