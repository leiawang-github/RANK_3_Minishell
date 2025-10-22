/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/10/22 17:41:01 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"


static int handle_in_redir(const char *file);
static int handle_out_redir(const char *file);
static int handle_append_redir(const char *file);
static int handle_heredoc_redir(t_redir *redir);
static int probe_in_redir(const char *file);
static int probe_out_redir(const char *file, int append);
static int probe_heredoc_redir(t_redir *redir);

int exec_redirs(t_redir *redirs)
{
    int rc;
    t_redir *redir;

    redir = redirs;
    while (redir)
    {
        if (redir->redir_type == IN)
            rc = handle_in_redir(redir->target);
        else if (redir->redir_type == OUT)
            rc = handle_out_redir(redir->target);
        else if (redir->redir_type == APPEND)
            rc = handle_append_redir(redir->target);
        else if (redir->redir_type == HEREDOC)
            rc = handle_heredoc_redir(redir);
        else
            rc = -1;

        if (rc < 0)
            return -1;
        redir = redir->next;
    }
    return 0;
}


/*
 * Open/validate redirections without altering the current process file
 * descriptors. This matches shells' behavior for a redirection-only
 * command like "> file" where the side-effect (create/truncate) is
 * applied but the shell's stdout is not permanently redirected.
 * Returns 0 on success, -1 on failure with error reported.
 */
int probe_redirs(t_redir *redirs)
{
    int rc;
    t_redir *redir;

    redir = redirs;
    while (redir)
    {
        if (redir->redir_type == IN)
            rc = probe_in_redir(redir->target);
        else if (redir->redir_type == OUT)
            rc = probe_out_redir(redir->target, 0);
        else if (redir->redir_type == APPEND)
            rc = probe_out_redir(redir->target, 1);
        else if (redir->redir_type == HEREDOC)
            rc = probe_heredoc_redir(redir);
        else
            rc = -1;

        if (rc < 0)
            return -1;
        redir = redir->next;
    }
    return 0;
}


static int handle_in_redir(const char *file)
{
	int fd;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return ft_errno(file, errno, ERR_BUILTIN);
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        return ft_errno(file, errno, ERR_BUILTIN);
    }
	close(fd);
	return 0;
}

static int handle_out_redir(const char *file)
{
	int fd;
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
         return ft_errno(file, errno, ERR_BUILTIN);
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        return ft_errno(file, errno, ERR_BUILTIN);
    }
	close(fd);
	return 0;
}

static int handle_append_redir(const char *file)
{
	int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
        return ft_errno(file, errno, ERR_BUILTIN);
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        return ft_errno(file, errno, ERR_BUILTIN);
    }
	close(fd);
	return 0;
}


static int handle_heredoc_redir(t_redir *redir)
{
    if (!redir)
         return err_msg("heredoc", ": internal error", ERR_BUILTIN);
    if (redir->fd < 0)
        return err_msg("heredoc", ": not prepared", ERR_BUILTIN);
    if (dup2(redir->fd, STDIN_FILENO) == -1)
    {
        close(redir->fd);
        return ft_errno("dup2", errno, ERR_BUILTIN);
    }
    close(redir->fd);
    redir->fd = -1;
    return 0;
}


/* Probe helpers: open/close to ensure side effects without dup2 */
static int probe_in_redir(const char *file)
{
    int fd;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return ft_errno(file, errno, ERR_BUILTIN);
    close(fd);
    return 0;
}

static int probe_out_redir(const char *file, int append)
{
    int fd;
    int flags;

    flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
    fd = open(file, flags, 0644);
    if (fd < 0)
        return ft_errno(file, errno, ERR_BUILTIN);
    close(fd);
    return 0;
}

static int probe_heredoc_redir(t_redir *redir)
{
    if (!redir)
        return err_msg("heredoc", ": internal error", ERR_BUILTIN);
    if (redir->fd < 0)
        return err_msg("heredoc", ": not prepared", ERR_BUILTIN);
    /* Do not dup2; just close the prepared fd */
    close(redir->fd);
    redir->fd = -1;
    return 0;
}

enum e_err
{
    ERR_NONE,   // 无错误
    ERR_PARSER, // 语法或解析错误
    ERR_EXEC,   // 执行阶段错误（比如 command not found）
    ERR_SYS,    // 系统调用错误（open、pipe、fork 等失败）
    ERR_BUILTIN // 内建命令错误
};
