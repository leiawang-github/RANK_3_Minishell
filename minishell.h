/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:11:44 by leiwang           #+#    #+#             */
/*   Updated: 2025/08/29 17:05:58 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <readline/readline.h>


//-------------------------------what I need possibly from parser-----------------------
//pipeline_list
typedef struct s_cmd {
    char     **argv; // first one will be cmd_name, the comes with arguments
    t_redir   *redirs; // a linked list, see below
    struct s_cmd *next;
} t_cmd;


typedef struct s_redir {
    t_redir_type redir_type;   // which kind of redirection
    char        *target; // file path or heredoc delimiter
    int          do_expand; // only for heredoc
    struct s_redir *next;   // pointer for linking multiple redirs
}   t_redir;

typedef struct s_redir {
    t_redir_type redir_type;   // which kind of redirection
    char        *target; // file path or heredoc delimiter
    int          do_expand; // only for heredoc
    struct s_redir *next;   // pointer for linking multiple redirs
}   t_redir;

typedef enum e_redir_type{
    R_REDIR_IN,
    R_REDIR_OUT,
    R_REDIR_APPEND,
    R_REDIR_HEREDOC
} t_redir_type;


int g_last_status;


//---------------------------------------------------------------------------------------





//lexer part
typedef enum e_token_type {
    T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APP,
	T_HEREDOC
} t_token_type;

typedef enum e_quote_type {
    Q_NONE = 0,
    Q_SQUOTE = 1,
    Q_DQUOTE = 2
} t_quote_type;

typedef struct s_token {
    t_token_type       type;
    t_quote_type   qoute_type;
    char           *lexeme;
    struct s_token *next;
} t_token;

typedef struct s_assign {
    char *key;            // "A"
    char *val;            // "1"（去引号、已做变量展开后）
    t_quote_type quote_type;        // 赋值右值的引号语义（需要的话）
    struct s_assign *next;
} t_assign;



//
typedef enum e_cmd_type {
    CMD_ASSIGN_ONLY,
    CMD_REDIR_ONLY,
    CMD_ASSIGN_AND_REDIR,
    CMD_WITH_ARGV,
    CMD_EMPTY
}   t_cmd_type;

typedef struct s_assign
{
    char            *key;
    char            *value;
    struct s_assign    *next;
}   t_assign;

typedef struct s_shell
{
    t_assign   *vars;   // shell 自己的变量表（非环境）
    char   **envp;   // 运行时环境变量表（传给 execve）
}   t_shell;


//executor functions
int     backup_fds(int bak[2]);
void    restore_fds(int bak[2]);
int     apply_redirs_in_parent(t_redir *r);
int     apply_redirs_in_child(t_redir *r);
void    apply_assigns_to_shell(t_assign *a);
void    apply_assigns_to_child_env(t_assign *a);
int     is_builtin(const char *name);
int     run_builtin(char **argv);
void    execve_with_path(const char *file, char *const argv[], char *const envp[]);
char  **get_envp(void);
int     interpret_status(int st);


int  exec_single(t_cmd *cmd);
int  exec_pipeline(t_cmd *head);
void child_exec_for_node(t_cmd *cmd);

int exec_single_cmd(head);



#endif
