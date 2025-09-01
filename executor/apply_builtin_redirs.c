/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_builtin_redirs.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/08/31 21:31:08 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int apply_redir_in(const char *path)
{
	
	int fd;
	
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		ft_perror(path);
		return -1;
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		ft_perror(path);
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int apply_redir_out(const char *path)
{
	int fd;
	
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (fd < 0)
	{
		ft_perror(path); //if open funct fails
		return -1;
	}
	if (dup2(fd, STDOUT_FILENO) < 0) // if open successfully, then redirect stdout to fd
	{
		ft_perror(path);
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}
int apply_redir_append(const char *path)
{
	int fd;

    fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644); //open file for write only, create if not exist, append if exist
	if (fd < 0)
	{
		ft_perror(path);
		return -1;
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		ft_perror(path);
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int apply_redir_heredoc(t_redir *redir) 
{
    if (!redir || redir->heredoc_fd < 0) {
        ft_perror(NULL);
        return -1;
    }
    if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0) {
        ft_perror(NULL);
        close(redir->heredoc_fd);
        redir->heredoc_fd = -1;
        return -1;
    }
    close(redir->heredoc_fd);
    redir->heredoc_fd = -1;
    return 0;
}


