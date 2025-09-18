/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/09/15 22:46:02 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

/* forward static prototypes */
static int handle_in_redir(const char *file);
static int handle_out_redir(const char *file);
static int handle_append_redir(const char *file);
static int handle_heredoc_redir(t_redir *redir);

int exec_redirs(t_redir *redirs)
{
    int rc;
    t_redir *redir;

    redir = redirs;
    while (redir)
    {
        if (redir->redir_type == R_REDIR_IN)
            rc = handle_in_redir(redir->file);
        else if (redir->redir_type == R_REDIR_OUT)
            rc = handle_out_redir(redir->file);
        else if (redir->redir_type == R_REDIR_APPEND)
            rc = handle_append_redir(redir->file);
        else if (redir->redir_type == R_REDIR_HEREDOC)
            rc = handle_heredoc_redir(redir);
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
        return ft_errno(file, errno, ERR_SYS_BUILTIN);
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        return ft_errno(file, errno, ERR_SYS_BUILTIN);
    }
	close(fd);
	return 0;
}

static int handle_out_redir(const char *file)
{
	int fd;
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
         return ft_errno(file, errno, ERR_SYS_BUILTIN);
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        return ft_errno(file, errno, ERR_SYS_BUILTIN);
    }
	close(fd);
	return 0;
}

static int handle_append_redir(const char *file)
{
	int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
        return ft_errno(file, errno, ERR_SYS_BUILTIN);
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        return ft_errno(file, errno, ERR_SYS_BUILTIN);
    }
	close(fd);
	return 0;
}


static int handle_heredoc_redir(t_redir *redir)
{
    if (!redir)
         return err_msg("heredoc", ": internal error", ERR_SYS_BUILTIN);
    if (redir->fd < 0)
        return err_msg("heredoc", ": not prepared", ERR_SYS_BUILTIN);
    if (dup2(redir->fd, STDIN_FILENO) == -1)
    {
        close(redir->fd);
        return ft_errno("dup2", errno, ERR_SYS_BUILTIN);
    }
    close(redir->fd);
    redir->fd = -1;
    return 0;
}
