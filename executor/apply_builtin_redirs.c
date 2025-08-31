/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_builtin_redirs.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/08/31 20:38:47 by leia             ###   ########.fr       */
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

int apply_redir_heredoc(t_redir *redir) {
    // 前置：r->heredoc_fd 由 prepare_heredocs 填好；若是 -1 代表无效
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


int prepare_heredocs(t_cmd *pipeline)
{
    while (pipeline)
    {
        t_redir *redir = pipeline->redirs;
        while (redir)
        {
            if (redir->redir_type == R_REDIR_HEREDOC)
            {
                int pipefd[2]; // [0] for read end, [1] for write end
                
                redir->heredoc_fd = -1; 
                if (pipe(pipefd) < 0)
                {
                    ft_perror(NULL);
                    return -1;
                }
                redir->heredoc_fd = pipefd[0]; // read end for later use
                char *line;
                while (1)
                {
                    line = readline("> ");
                    if (!line || ft_strcmp(line, redir->delimiter) == 0)
                    {
                        free(line);
                        break;
                    }
                    if (redir->do_expand)
                        line = expand_variables(line); // implement this function as needed
                    write(pipefd[1], line, ft_strlen(line));
                    write(pipefd[1], "\n", 1);
                    free(line);
                }
                close(pipefd[1]); // close write end after done
            }
            redir = redir->next;
        }
        pipeline = pipeline->next;
    }
    return 0;
}