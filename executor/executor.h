/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:00:52 by leia              #+#    #+#             */
/*   Updated: 2025/08/31 21:30:53 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
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
    char        *delimiter; // this is the char* after remove quotes(either single or double),ex: "EOF" -> EOF 
    int          do_expand; // only for heredoc one case: if delimiter is quoted, do_expand = 0; else do_expand = 1
    int          heredoc_fd; // the read end of the pipe for heredoc   
    struct s_redir *next;   // pointer for linking multiple redirs
}   t_redir;

typedef enum e_redir_type{
    R_REDIR_IN,
    R_REDIR_OUT,
    R_REDIR_APPEND,
    R_REDIR_HEREDOC
} t_redir_type;


int g_last_status;

int exec_single_cmd(pipeline);
int exec_pipeline(pipeline);

int  apply_redirs_in_parent(t_cmd *pipeline);
int is_builtin(const char *name);

int apply_redir_in(const char *path);
int apply_redir_out(const char *path);
int apply_redir_append(const char *path);
int apply_redir_heredoc(t_redir *redir);

int prepare_heredocs(t_cmd *pipeline);


#endif