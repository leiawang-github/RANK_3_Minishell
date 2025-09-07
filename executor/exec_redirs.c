/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/09/05 08:31:34 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int   exec_redir_only(t_cmd *pipeline)  //single node, only redirections
{
	int rc;  //means return code
    t_redir *redir;
    redir = pipeline->redirs;

	while (redir)
	{
		if (redir->redir_type == R_REDIR_IN) // < 语义：把文件内容作为命令的标准输入。
			rc = handle_in_redir(redir->file);
		else if (redir->redir_type == R_REDIR_OUT) // > 语义：把命令的标准输出写入文件，覆盖原有内容。
			rc = handle_out_redir(redir->file);
		else if (redir->redir_type == R_REDIR_APPEND) // >> 语义：把命令的标准输出写入文件，追加到文件末尾。
			rc  = handle_append_redir(redir->file);
		else if (redir->redir_type == R_REDIR_HEREDOC) // <<
			rc  = handle_heredoc_redir(redir->delimiter);
		if (rc < 0)
            return (-1);
        redir = redir->next;
	}
	return (0);
}

static int handle_in_redir(const char *file) 
{
	int fd;
    
    fd = open(file, O_RDONLY);
	if (fd < 0) 
    { 
        ft_perror(file); 
        return -1; 
    }
	close(fd);
	return 0;
}

static int handle_out_redir(const char *file) 
{
	int fd;
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) 
    { 
        ft_perror(file); 
        return -1; 
    }
	close(fd);
	return 0;
}

static int handle_append_redir(const char *file) 
{
	int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0) 
    { 
        ft_perror(file); 
        return -1; 
    }
	close(fd);
	return 0;
}

static int handle_heredoc_redir(const char *delimiter) 
{
    int pipefd[2];
    char *line;

    if (pipe(pipefd) == -1) 
    { 
        ft_perror("pipe"); 
        return -1; 
    }

    while (1) 
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0) 
        {
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]); // 关闭写端
    return pipefd[0]; // 返回读端
}