/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:09 by leia              #+#    #+#             */
/*   Updated: 2025/10/19 17:51:39 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

int  install_heredoc_parent_signals(void);
int  install_heredoc_child_signals(void);
int  exact_equal(const char*, const char*);
void warn_hdoc_eof(const char*);
char* strip_quotes(const char*);
int  has_any_quote(const char*);
void expand_vars_inplace(char **line, char **envp);
char**get_envp_snapshot(void);
void mark_fd_for_cleanup_later(int fd);
void close_all_marked_heredoc_fds(void);
int  ft_errno(const char *file, int saved_errno, enum e_err kind);
int  err_msg(const char *prefix, const char *suffix, enum e_err kind);

int prepare_all_heredocs(t_mini *head)
{
    t_mini *cmd;
    t_redir *redir;
    int rc;

    cmd = head;
    install_heredoc_parent_signals();
    while (cmd)
    {
        redir = cmd->redirs;
        while (redir)
        {
            if (redir->redir_type == HEREDOC)
            {
                rc = heredoc_prepare_one(redir);
                if (rc != 0)
                {
                    restore_interactive_signals();
                    return rc;
                }
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return 0;
}


static int heredoc_collect_loop(t_redir *redir, int wfd)
{
    char *line;
    while (1)
    {
        line = readline("> ");
        if (line == NULL) { warn_hdoc_eof(redir->delimiter); break; }
        if (exact_equal(line, redir->delimiter)) { free(line); break; }
        if (redir->hdoc_expand) {
            char **snap = get_envp_snapshot();
            expand_vars_inplace(&line, snap);
            /* 如需释放 snap，放你实现里 */
        }
        dprintf(wfd, "%s\n", line);
        free(line);
    }
    return 0;
}

static int heredoc_prepare_one(t_redir *redir)
{
    int pfd[2], st;
    pid_t pid;

    redir->delimiter = strip_quotes(redir->target);
    redir->hdoc_expand = !has_any_quote(redir->target);
    redir->heredoc_fd = -1;
    if (pipe(pfd) < 0)
        return (ft_errno("pipe", errno, ERR_SYS));
    pid = fork();
    if (pid < 0)
    {
        close(pfd[0]);
        close(pfd[1]);
        return(ft_errno("fork", errno, ERR_SYS));
    }
    if (pid == 0)
    {
        install_heredoc_child_signals();
        close(pfd[0]);
        (void)heredoc_collect_loop(redir, pfd[1]);
        close(pfd[1]); _exit(0);
    }
    close(pfd[1]);
    if (waitpid(pid, &st, 0) < 0)
        {
            close(pfd[0]);
            return ft_errno("waitpid", errno, ERR_SYS);
        }
    if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
    {
        close(pfd[0]);
        close_all_marked_heredoc_fds();
        return 130;
    }
    if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
    {
        close(pfd[0]);
        return err_msg("heredoc: ", "child failed", ERR_EXEC);
    }
    redir->heredoc_fd = pfd[0];
    mark_fd_for_cleanup_later(pfd[0]);
    return 0;
}

