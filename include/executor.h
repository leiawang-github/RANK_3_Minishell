/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:00:52 by leia              #+#    #+#             */
/*   Updated: 2025/09/14 11:54:25 by leia             ###   ########.fr       */
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

/* Some environments (e.g., libedit) may not expose these prototypes */
#ifndef RL_API_DECLARED
#define RL_API_DECLARED 1
extern int  rl_on_new_line(void);
extern void rl_replace_line(const char *text, int clear_undo);
extern void rl_redisplay(void);
#endif

/* Redirection types */
typedef enum e_redir_type {
    R_REDIR_NONE = 0,
    R_REDIR_IN,
    R_REDIR_OUT,
    R_REDIR_APPEND,
    R_REDIR_HEREDOC
} t_redir_type;

/* Forward decls */
typedef struct s_redir t_redir;
typedef struct s_cmd   t_cmd;

/* Redir node */
struct s_redir {
    t_redir_type   redir_type;   /* which kind of redirection */
    char          *file;         /* for <, >, >> */
    char          *delimiter;    /* for << (after quote removal) */
    int            do_expand;    /* heredoc: expand variables? */
    int            fd;           /* heredoc pipe read-end (preprocessed) */
    struct s_redir *next;
};

/* Command node */
struct s_cmd {
    char     **argv;   /* argv[0] is command name */
    t_redir   *redirs; /* linked list of redirections */
    t_cmd     *next;   /* next command in pipeline */
};

/* Command classification */
typedef enum e_cmd_type {
    CMD_REDIR_ONLY,  /* empty command with only redirections */
    CMD_BUILTIN,
    CMD_EXTERNAL,
    CMD_INVALID
} t_cmd_type;

enum e_err {
    ERR_NONE,
    ERR_SYNTAX,
    ERR_CMD_NOT_FOUND,
    ERR_CANNOT_EXEC,     // perm denied / is a directory / exec format
    ERR_SYS_BUILTIN,     // 内建的系统错误
    ERR_REDIR,           // 打开重定向失败
    ERR_SIGINT, ERR_SIGQUIT
};


/* Global last status (provided by shell core) */
extern int g_last_status;

/* Analyzer */
t_cmd_type analyze_cmd(t_cmd *cmd);

int builtin_implementation(t_cmd *pipeline);

/* Pipeline dispatcher */
int execute_command(t_cmd *pipeline, char **envp);

/* Single/pipeline executors */
int exec_redir_only(t_cmd *node);
int exec_builtin_in_single_cmd(t_cmd *pipeline);
int exec_single_external(t_cmd *node, char **envp);
int exec_pipeline(t_cmd *head, char **envp);

/* Heredoc preprocess */
int preprocess_heredoc(t_cmd *pipeline, char **envp, int *interrupted);

/* Error helper */
int ft_errno(const char *file, int saved_errno);
int err_msg(const char *msg);

//implementation of builtins
int builtin_echo(char **argv);
int builtin_cd(char **argv);
int builtin_pwd(void);
int builtin_export(char **argv);
int builtin_unset(char **argv);
int builtin_env(void);
int builtin_exit(char **argv);

/* Signal handlers */
void sigint_handler(int sig);



int is_valid_var_name(const char *name);


#endif
