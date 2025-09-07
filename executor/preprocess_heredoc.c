/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:09 by leia              #+#    #+#             */
/*   Updated: 2025/09/07 15:35:42 by leia             ###   ########.fr       */
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

int preprocess_heredoc(t_cmd *pipeline, char **envp, int *interrupted) //疑问：有必要采用node变量吗
{
    t_cmd *node; 
    t_redir *redir;

    node = pipeline;
    if (interrupted) 
        *interrupted = 0;
    while (node) 
    {
        if(!node->redirs) 
        {
            node = node->next;
            continue;
        }
        redir = node->redirs;
        while (redir) 
        {
            if (redir->redir_type == R_REDIR_HEREDOC) 
            {
                if (prepare_one_heredoc(redir, envp, interrupted) != 0)  //prepare_one_heredoc 采集失败
                {
                    if (interrupted && *interrupted) //被 Ctrl-C 打断
                        return 130;
                    return -1;
                }
            }
            redir = redir->next;
        }
        node = node->next;
    }
    return 0;
}

static int prepare_one_heredoc(t_redir *redir, char **envp, int *interrupted) 
{
    int fds[2]; //fd[0]读端，fd[1]写端，这是所有管道的标配
    pid_t child_pid;
    int status; //采集子进程的退出状态
    int return_code;

    if (interrupted) 
        *interrupted = 0;
    if (pipe(fds) < 0) //建立一条匿名管道失败
        return (g_last_status = (ft_perror("pipe"), 1));
    signal(SIGINT, SIG_IGN); // 父进程忽略 SIGINT ctrl-c，为什么是父进程？因为还没有fork
    signal(SIGQUIT, SIG_IGN); // 父进程忽略 SIGQUIT
    child_pid = fork(); //成功会返回子进程 ID，失败返回 -1，子进程返回 0
    if (child_pid < 0)  //如果 fork 失败
    {
        ft_perror("fork");
        close(fds[0]);
        close(fds[1]);
        return (g_last_status = 1);
    }
    if (child_pid == 0) // 如果子进程创建成功
    {
        signal(SIGINT, SIG_DFL); // 采集子进程恢复默认 SIGINT 处理
        signal(SIGQUIT, SIG_DFL); // 采集子进程恢复默认 SIGQUIT 处理
        close(fds[0]); // 关闭读端
        return_code = heredoc_collect_loop(redir, fds[1], envp); // 采集多行文本
        close(fds[1]); // 关闭写端
        _exit(return_code); // 正常结束
    }
    /* 父进程：等待采集子进程结束 */
    close(fds[1]); // 关闭写端，父进程是消费者，只需要读端
    if (waitpid(child_pid, &status, 0) == -1) 
    {
        int saved = errno;
        close(fds[0]);
        errno = saved;
        return ft_perror("waitpid");
    }
    /* 采集子进程结束后，恢复父进程的 SIGINT 处理 */
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
    if (WIFSIGNALED(status)) //采集子进程被信号终止
    {
        if (WTERMSIG(status) == SIGINT && interrupted) //被 Ctrl-C 打断
            *interrupted = 1;
        close(fds[0]); //关闭读端
        return (g_last_status = 130);
    }
    if (WIFEXITED(status)) 
    {
        int rc = WEXITSTATUS(status) & 0xFF;
        if (rc != 0) 
        {
            close(fds[0]);
            return (g_last_status = rc);
        }
    }
        redir->fd = fds[0];
        return 0;
}

static void handle_sigint(int sig)
{
	(void)sig;
    g_last_status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static int	write_all(int fd, const char *buf, size_t len)
{
	size_t done = 0;
	while (done < len)
	{
		ssize_t w = write(fd, buf + done, len - done);
		if (w < 0)
		{
			if (errno == EINTR)
				continue;
			return (-1);
		}
		done += (size_t)w;
	}
	return (0);
}

/* 变量展开占位：按需替换为你的展开实现（$VAR / $?）。不展开时原样拷贝。*/
static char	*expand_heredoc_line(const char *line, char **envp, int do_expand)
{
	(void)envp;
	if (!line)
		return (NULL);
	if (!do_expand)
		return (strdup(line));
	/* TODO: 用你的 expand 实现替换这里 */
	return (strdup(line));
}

/* heredoc 采集主循环：使用 redir->delimiter / redir->do_expand，逐行写入 write_fd，每行补 '\n' */
int	heredoc_collect_loop(t_redir *redir, int write_fd, char **envp)
{
	char	*line;
	char	*out;

	if (!redir || !redir->delimiter)
		return (1);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			/* 提前 EOF：可选 warning，贴近 bash */
			/* dprintf(2, "minishell: warning: here-document delimited by end-of-file\n"); */
			return (0);
		}
		/* 碰到结束标志（严格等于，不含换行） */
		if (ft_strcmp(line, redir->delimiter) == 0)
		{
			free(line);
			return (0);
		}
		/* 按需展开 */
		out = expand_heredoc_line(line, envp, redir->do_expand);
		free(line);
		if (!out)
			return (1);
		/* 写入一行 + '\n' */
		if (write_all(write_fd, out, ft_strlen(out)) < 0 || write_all(write_fd, "\n", 1) < 0)
		{
			free(out);
			return (1);
		}
		free(out);
	}
}

char expand_heredoc_line(const char *line, char **envp, int do_expand)
{
    
    (void)envp;
    if (!line)
        return (NULL);
    if (!do_expand)
        return (strdup(line));  
}
  
/* 仅 $VAR / $? 展开；do_expand==0 时原样返回一份拷贝 */
static char *expand_heredoc_line(const char *line, char **envp, int do_expand);

int heredoc_collect_loop(t_redir *redir, int wfd, char **envp)
{
	char *line;

	if (!redir || !redir->delimiter) return 1;
	while (1) 
    {
		line = readline("heredoc> ");
		if (!line) 
            return 0; /* EOF */
		if (ft_strcmp(line, redir->delimiter) == 0) 
        { 
            free(line); 
            return 0; 
        }
		const char *to_write = line;
		char *expanded = NULL;
		if (redir->do_expand)
        {
			expanded = expand_heredoc_line(line, envp, 1);
			if (!expanded) 
            { 
                free(line); 
                return 1; 
            }
			to_write = expanded;
		}
		if (write(wfd, to_write, strlen(to_write)) < 0 || write(wfd, "\n", 1) < 0) 
        {
			if (expanded) free(expanded);
			free(line);
			return 1;
		}
		if (expanded) free(expanded);
		free(line);
	}
}

