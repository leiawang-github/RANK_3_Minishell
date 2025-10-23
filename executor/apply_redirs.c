/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/10/24 01:30:26 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

int	apply_all_redirs(t_redir *redirs)
{
	t_redir	*cur;

	cur = redirs;
	while (cur)
	{
		if (apply_one_redir(cur) != 0)
			return (-1);
		cur = cur->next;
	}
	return (0);
}

static int	apply_one_redir(t_redir *redir)
{
	if (!redir)
		return (-1);
	if (redir->redir_type == IN)
		return (redir_in(redir));
	if (redir->redir_type == OUT || redir->redir_type == APPEND)
		return (redir_out_or_append(redir));
	if (redir->redir_type == HEREDOC)
		return (redir_heredoc(redir));
	return (0);
}

static int	redir_in(t_redir *redir)
{
	int	fd;

	if (!redir || !redir->target)
		return (-1);
	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_out_or_append(t_redir *redir)
{
	int	fd;
	int	flags;

	if (!redir || !redir->target)
		return (-1);
	flags = O_CREAT | O_WRONLY;
	if (redir->redir_type == OUT)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	fd = open(redir->target, flags, 0644);
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	redir_heredoc(t_redir *redir)
{
	if (!redir)
		return (-1);
	if (redir->heredoc_fd < 0)
		return (-1);
	if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
	{
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
		return (-1);
	}
	close(redir->heredoc_fd);
	redir->heredoc_fd = -1;
	return (0);
}
