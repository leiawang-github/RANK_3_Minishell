/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:09 by leia              #+#    #+#             */
/*   Updated: 2025/10/22 17:33:25 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

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

static int	prepare_one_heredoc(t_redir *redir);
static void	heredoc_child(t_redir *redir, int *pfd);
static int	fail_fork_close(int *pfd);
static void	hdoc_collect(char *delim, int fd, int expand);


int prepare_all_heredocs(t_mini *head)
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

static int	prepare_one_heredoc(t_redir *redir)
{
	int		pfd[2];
	int		st;
	pid_t	pid;
	void	(*old_ign)(int);
	void	(*old_quit)(int);

	if (!redir)
		return (g_last_status = 1);
	if (pipe(pfd) < 0)
		return (g_last_status = 1);
	pid = fork();
	if (pid < 0)
		return (fail_fork_close(pfd));
	if (pid == 0)
		heredoc_child(redir, pfd);
	set_parent_ignore_signals(&old_ign, &old_quit);
	close(pfd[1]);
	if (waitpid(pid, &st, 0) == -1)
		return (fail_wait_cleanup(pfd, old_ign, old_quit));
	restore_parent_signals(old_ign, old_quit);
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
    {
        close(pfd[0]);
        return (g_last_status = 130);
    }
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
    {
        close(pfd[0]);
        return ( g_last_status = 1);
    }
	redir->heredoc_fd = pfd[0];
	return (g_last_status = 0);
}

static void	heredoc_child(t_redir *redir, int *pfd)
{
	signal(SIGINT, SIG_DFL); // take ctrl+c as usual
	signal(SIGQUIT, SIG_IGN); //ingore ctrl+\ since in interractive mode in bash
	close(pfd[0]);
	hdoc_collect(redir->delimiter, pfd[1], redir->hdoc_expand);
	close(pfd[1]);
	_exit(0);
}

static int	fail_fork_close(int *pfd)
{
	close(pfd[0]);
	close(pfd[1]);
	return (g_last_status = 1);
}

static void	hdoc_collect(char *delim, int fd, int hdoc_expand)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (hdoc_expand)
			line = expand_vars(line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}
