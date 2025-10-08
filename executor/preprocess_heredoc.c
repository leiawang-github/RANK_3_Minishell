/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:46:09 by leia              #+#    #+#             */
/*   Updated: 2025/09/07 17:56:54 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

/*
  Heredoc preprocess design:
  - For every << redirection, fork a child that collects lines with readline.
  - Parent ignores SIGINT while waiting; child uses default SIGINT/SIGQUIT.
  - On success, parent stores the read end to redir->fd for later dup2.
*/

/* forward decls */
static int  prepare_one_heredoc(t_redir *redir, char **envp, int *interrupted);
static int  write_all(int fd, const char *buf, size_t len);
static char *expand_heredoc_line(const char *line, char **envp, int do_expand);
static int  heredoc_collect_loop(t_redir *redir, int write_fd, char **envp);

int preprocess_heredoc(t_mini *pipeline, char **envp, int *interrupted)
{
    int   rc;
    t_mini *node = pipeline;

    if (interrupted)
        *interrupted = 0;
    while (node)
    {
        t_redir *r = node->redirs;
        while (r)
        {
            if (r->redir_type == HEREDOC)
            {
                rc = prepare_one_heredoc(r, envp, interrupted);
                if (rc != 0)
                    return rc; /* 130 if interrupted, else error */
            }
            r = r->next;
        }
        node = node->next;
    }
    return 0;
}

static int prepare_one_heredoc(t_redir *redir, char **envp, int *interrupted)
{
    int   fds[2];
    pid_t child;
    int   status;

    if (interrupted)
        *interrupted = 0;
    if (pipe(fds) < 0)
    {
        ft_errno("pipe", errno, ERR_SYS_BUILTIN);
        return (g_last_status = 1);
    }

    /* Parent: ignore Ctrl-C/Q while collecting */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    child = fork();
    if (child < 0)
    {
        ft_errno("fork", errno, ERR_SYS_BUILTIN);
        close(fds[0]);
        close(fds[1]);
        return (g_last_status = 1);
    }
    if (child == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(fds[0]);
        int rc = heredoc_collect_loop(redir, fds[1], envp);
        close(fds[1]);
        _exit(rc);
    }

    /* Parent: wait for child */
    close(fds[1]);
    if (waitpid(child, &status, 0) == -1)
    {
        int saved = errno;
        close(fds[0]);
        errno = saved;
        return ft_errno("waitpid", errno, ERR_SYS_BUILTIN);
    }
    /* Restore REPL handler semantics: SIGINT custom, SIGQUIT ignored */
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);

    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT && interrupted)
            *interrupted = 1;
        close(fds[0]);
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



static int write_all(int fd, const char *buf, size_t len)
{
    size_t written = 0;
    while (written  < len)
    {
        ssize_t w = write(fd, buf + written , len - written );
        if (w < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        written  += (size_t)w;
    }
    return 0;
}

static int heredoc_collect_loop(t_redir *redir, int write_fd, char **envp)
{
    char *line;
    char *expander; // expanded line

    if (!redir || !redir->delimiter)
        return 1;
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
        expander = expand_heredoc_line(line, envp, redir->do_expand);
        free(line);
        if (!expander)
            return 1;
        if (write_all(write_fd, expander, ft_strlen(expander)) < 0 || write_all(write_fd, "\n", 1) < 0)
        {
            free(expander);
            return 1;
        }
        free(expander);
    }
}

static char *expand_heredoc_line(const char *line, char **envp, int do_expand)
{
    (void)envp; /* TODO: hook your real expander */
    if (!line) 
        return NULL;
    if (!do_expand) 
        return (ft_strdup(line));
    return (ft_strdup(line));
}

