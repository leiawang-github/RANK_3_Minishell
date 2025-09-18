/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:00:52 by leia              #+#    #+#             */
/*   Updated: 2025/09/16 20:39:01 by leia             ###   ########.fr       */
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

#include "env_copy.h"
#include "minishell_def.h"

/* Forward decls */
typedef struct s_cmd   t_cmd;

/* Command node */
struct s_cmd {
    char     **argv;   /* argv[0] is command name */
    t_redir   *redirs; /* linked list of redirections */
    t_cmd     *next;   /* next command in pipeline */
};

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

enum e_redir
{
    R_REDIR_IN,
    R_REDIR_OUT,
    R_REDIR_APPEND,
    R_REDIR_HEREDOC
};

/* Global last status (provided by shell core) */
extern int g_last_status;

/* Analyzer */
t_cmd_type analyze_cmd(t_cmd *cmd);


/* Pipeline dispatcher */
int execute_command(t_cmd *pipeline, char **envp, t_env *env_list);

/* Single/pipeline executors */
int exec_redirs(t_redir *redirs);
int exec_builtin_in_single_cmd(t_cmd *pipeline, t_env *env_list);
int exec_single_external(t_cmd *pipeline, t_env *env_list, char **envp);
int exec_pipeline(t_cmd *head, char **envp, t_env *env_list);

/* Path resolution */
char *which_path(char *cmd_name, t_env *env_list);

/* Heredoc preprocess */
int preprocess_heredoc(t_cmd *pipeline, char **envp, int *interrupted);

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



int is_valid_var_name(const char *name);


#endif

