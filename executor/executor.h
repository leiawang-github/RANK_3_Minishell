/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:00:52 by leia              #+#    #+#             */
/*   Updated: 2025/09/06 06:26:26 by leia             ###   ########.fr       */
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
#include <sys/wait.h>
#include <readline/readline.h>

#ifndef READ_END
# define READ_END  0
#endif
#ifndef WRITE_END
# define WRITE_END 1
#endif

typedef struct s_executor {
    t_cmd      *cmds;      // 命令链表
     char       *path;      // 完整路径（执行时查找）
    char        **envp;         // 环境变量
    t_cmd_type type;     // 命令类型
    t_redir   *redirs;   // 重定向信息
    int         status;         // 执行状态
    pid_t       *pids;          // 子进程PID数组
    int         pipe_count;     // 管道数量
} t_executor;

typedef struct s_cmd {
    char     **argv; // first one will be cmd_name, the comes with arguments
    t_redir   *redirs; // a linked list, see below
    struct s_cmd *next;
} t_cmd;


typedef struct s_redir 
{
    t_redir_type redir_type;   // which kind of redirection
    char        *file; // file path or heredoc delimiter
    char        *delimiter; // this is the char* after remove quotes(either single or double),ex: "EOF" -> EOF 
    int          do_expand; // only for heredoc one case: if delimiter is quoted, do_expand = 0; else do_expand = 1
    int          fd; // file descriptor after open 
    struct s_redir *next;   // pointer for linking multiple redirs
}   t_redir;

typedef enum e_redir_type
{
    R_REDIR_NONE,
    R_REDIR_IN,
    R_REDIR_OUT,
    R_REDIR_APPEND,
    R_REDIR_HEREDOC
} t_redir_type;

typedef enum e_cmd_type 
{
    CMD_REDIR_ONLY,      // 空命令（只有重定向）
    CMD_BUILTIN,    // 内置命令
    CMD_EXTERNAL,   // 外部命令
    CMD_INVALID     // 无效命令
} t_cmd_type;

extern int g_last_status;

t_cmd_type analyze_cmd(t_cmd *cmd); 
int is_builtin(const char *file);

 

int exec_single_cmd(pipeline);
int exec_pipeline(pipeline);


int   exec_redir_only(t_cmd *pipeline);
int   exec_builtin(pipeline, envp);
int   exec_single_external(pipeline, *envp);
int   execute_command(t_cmd *pipeline, char **envp);
 

int ft_perror(const char *where);


#endif