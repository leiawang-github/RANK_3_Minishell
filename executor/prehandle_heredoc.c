/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:09 by leia              #+#    #+#             */
/*   Updated: 2025/09/01 15:24:02 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/*
<< 的完整时间线（单命令示意）

以 cat << EOF 为例：

**父进程（Shell 主进程）**开始预处理 heredoc：pipe(fds)。

计划把 fds[0]（读端）留下来给后续命令当 STDIN，

fds[1]（写端）用来接收采集到的多行文本。

父进程临时把 SIGINT 设为忽略：signal(SIGINT, SIG_IGN)（只在采集期间）。
目的：等会儿用户按 Ctrl-C 时，不要把主壳干掉。

父进程 fork 出“heredoc 采集子进程”。

子进程把 SIGINT 设为默认：signal(SIGINT, SIG_DFL)，开始循环 readline("> ")；
每行必要时做 $ 展开 → 写到 fds[1]；遇到定界词 EOF 就 _exit(0) 结束。

父进程 waitpid(child, &st, 0) 等这个采集子进程结束（期间仍然忽略 SIGINT）。

用户此时如果按下 Ctrl-C（SIGINT）：

内核把 SIGINT 发送给前台进程组（父、子都在其中）。

采集子进程因为 SIG_DFL → 被信号终止；

父进程因为临时 SIG_IGN → 不退出。

waitpid 返回“因信号终止”，父进程据此：

设 g_last_status = 130；

关闭所有已建立的 heredoc_fd；

放弃本条命令的后续执行，回到提示符。

如果采集正常结束：

父进程关闭 fds[1]（写端），把 fds[0]（读端）保存到 redir->heredoc_fd；

恢复 SIGINT 处理为 REPL 的 handler；

进入“执行阶段”：该 simple cmd 的子进程里 dup2(r->heredoc_fd, STDIN_FILENO)，然后 execve（或跑内建）。

对比：< file 没有“采集阶段”，只需 open + dup2，因此无需这套信号舞步。

*/

int prehandle_heredocs(t_cmd *pipeline)
{
    t_redir      *redir;

    if (!pipeline)
        return (0);
    heredoc_init_all_fds(pipeline);
    while (pipeline)
    {
        redir = pipeline->redirs;
        while (redir)
        {
            if (redir->redir_type == R_REDIR_HEREDOC)
                if (heredoc_build_fd_for_redir(redir) < 0)
                    return (hd_close_all_fds(pipeline), -1);
            redir = redir->next;
        }
        pipeline = pipeline->next;
    }
    return (0);
}

static void heredoc_init_all_fds(t_cmd *pipeline)
{
    t_cmd *pipeline;
    t_redir      *redir;
    
    while (pipeline)
    {
        redir = pipeline->redirs;
        while (redir)
        {
            if (redir->redir_type == R_REDIR_HEREDOC)
                redir->heredoc_fd = -1;
            redir = redir->next;
        }
        pipeline = pipeline->next;
    }
}


static int  heredoc_build_fd_for_redir(t_redir *redir)
{
    int         fds[2];
    const char *delim;

    if (pipe(fds) < 0)
        return (ft_perror("pipe"), -1);
    delim = (redir->delimiter && *redir->delimiter) ? redir->delimiter : redir->target;
    if (heredoc_read_into_pipe(fds[1], delim, redir->do_expand) < 0)
    {
        close(fds[1]);
        close(fds[0]);
        return (-1);
    }
    close(fds[1]);
    redir->heredoc_fd = fds[0];
    return (0);
}

static int  heredoc_read_into_pipe(int write_fd, const char *delim, int do_expand)
{
    pid_t          pid;
    int            st;
    void         (*oldint)(int);

    pid = fork();
    if (pid < 0)
        return (ft_perror("fork"), -1);
    if (pid == 0)
        heredoc_child_loop(write_fd, delim, do_expand);
    oldint = signal(SIGINT, SIG_IGN);
    waitpid(pid, &st, 0);
    signal(SIGINT, oldint);
    if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
    {
        g_last_status = 130;
        return (-1);
    }
    if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
        return (0);
    return (-1);
}

static void heredoc_child_loop(int write_fd, const char *delim, int do_expand)
{
    char *line;

    signal(SIGINT, SIG_DFL); //默认signal，如果按下ctrl+c，直接终止子进程
    signal(SIGQUIT, SIG_DFL); //默认signal，如果按下ctrl+D，直接终止子进程
    {
        line = readline("> ");
        if (line == NULL) //当用户按下ctrl+D时，readline返回NULL
        {
            heredoc_warn_eof(delim);
            _exit(0);
        }
        if (ft_strcmp(line, delim) == 0) //如果用户输入的行是定界词
        {
            free(line);
            _exit(0);
        }
        line = heredoc_expand(line, do_expand);
        if (heredoc_write_line(write_fd, line) < 0)
        {
            free(line);
            _exit(1);
        }
        free(line);
    }
}


static void heredoc_warn_eof(const char *delim)
{
    fprintf(stderr,
        "warning: here-document delimited by end-of-file (wanted `%s')\n",
        delim);
}

static int heredoc_write_line(int write_fd, const char *str)
{
    size_t n = 0;

    if (str != NULL)
        n = ft_strlen(str);
    if (n > 0 && write(write_fd, str, n) < 0)
        return (-1);
    if (write(write_fd, "\n", 1) < 0)
        return (-1);
    return (0);
}

// 约定：expand_variables() 返回“新分配”的串；失败返回 NULL。
// 你也可把 environ / g_last_status 作为额外参数传进去。
char *heredoc_expand(char *line, int do_expand)
{
    char *expanded;

    if (!line || !do_expand)
        return (line);
    expanded = expand_variables(line, environ /*, g_last_status */);
    if (!expanded)
        return (line);        // 失败：保留原串，避免丢失
    free(line);               // 成功：释放旧串，返回新串
    return (expanded);
}
