/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:09 by leia              #+#    #+#             */
/*   Updated: 2025/10/22 00:06:18 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

/*
流程：
[1] 建立承载
  └─ pipe(pfd)
       pfd[1] = 子进程写端
       pfd[0] = 父进程读端（稍后赋给 r->heredoc_fd）

[2] fork()
  ├─ 子进程（专职采集）
  │   ├─ 信号：SIGINT=DFL（可被 Ctrl-C 杀掉），SIGQUIT=IGN
  │   ├─ close(pfd[0])       // 子不用读端
  │   └─ 读入循环（只用 while）：
  │        while (1):
  │          line = readline("> ")
  │          if line == NULL:          // Ctrl-D
  │               warn_hdoc_eof(r->delimiter); break
  │          if line == r->delimiter:
  │               free(line); break
  │          if r->do_expand:
  │               expand_vars_inplace(&line, env_snapshot)
  │          write(line + '\n') → pfd[1]
  │          free(line)
  │        close(pfd[1]); _exit(0)

  └─ 父进程（协调/交付）
      ├─ 临时忽略 SIGINT（保存旧 handler 后设 SIG_IGN）
      ├─ close(pfd[1])            // 父不用写端
      ├─ waitpid(child, &st, 0)   // 等待采集结束
      ├─ 恢复父进程原 SIGINT handler
      ├─ 若子进程因 SIGINT 终止：
      │     close(pfd[0]); g_last_status=130; 返回 130  // 取消整条命令
      ├─ 若子异常退出或其它错误：
      │     close(pfd[0]); g_last_status=1;   返回 1
      └─ 成功：
            r->heredoc_fd = pfd[0]            // 交付给执行阶段 dup2(STDIN)
            （可登记清理表）g_last_status=0; 返回 0
*/

static int prepare_all_heredocs(t_mini *head)
{
    t_mini  *cmd;
    t_redir *redir;
    int      rc;

    cmd = head;
    while (cmd)
    {
        redir = cmd->redirs;
        while (redir)
        {
            if (redir->redir_type == HEREDOC)
            {
                rc = prepare_one_heredoc(redir);
                if (rc != 0)
                    return rc;
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return 0;
}

int prepare_one_heredoc(t_redir *redir)
{
    int pfd[2]; //create a pipe to communicate between parent and child processes
    int st;
    pid_t pid; //return value of fork func
    struct sigaction ign = {0}, dfl = {0}, qign = {0}, old = {0};

    if (pipe(pfd) < 0)
    {
        g_last_status = 1;
        return 1;
    }
    pid = fork();
    if (pid < 0)
    {
        close(pfd[0]);
        close(pfd[1]);
        g_last_status = 1;
        return 1;
    }
    if (pid == 0) //successfully fork child process
    {
        dfl.sa_handler = SIG_DFL;
        qign.sa_handler = SIG_IGN;
        sigaction(SIGINT, &dfl, NULL);
        sigaction(SIGQUIT, &qign, NULL);
        close(pfd[0]);
        hdoc_collect(redir->delimiter, pfd[1], redir->hdoc_expand);
        close(pfd[1]);
        _exit(0);
    }
    ign.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ign, &old);
    close(pfd[1]);
    waitpid(pid, &st, 0);
    sigaction(SIGINT, &old, NULL);
    if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
    {
        close(pfd[0]);
        g_last_status = 130;
        return 130;
    }
    if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
    {
        close(pfd[0]);
        g_last_status = 1;
        return 1;
    }
    redir->heredoc_fd = pfd[0];
    g_last_status = 0;
    return 0;
}

static int hdoc_collect(const char *delim, int wfd, int hdoc_expand)
{
    char *line;
    while (1)
    {
        line = readline("> ");
        if (line == NULL)
            break;
        if (ft_strcmp(line, delim) == 0)
        {
            free(line);
            break;
        }
        if (hdoc_expand)
            expand_vars_inplace(&line, get_envp_snapshot());
        write(wfd, line, ft_strlen(line));
        write(wfd, "\n", 1);
        free(line);
    }
    return 0;
}
