/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/09/10 00:03:53 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/* forward static prototypes */
static int handle_in_redir(const char *file);
static int handle_out_redir(const char *file);
static int handle_append_redir(const char *file);
static int handle_heredoc_redir(t_redir *redir);

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
			rc  = handle_heredoc_redir(redir);
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
        return err_file_errno(file, errno);
	close(fd);
	return 0;
}

static int handle_out_redir(const char *file) 
{
	int fd;
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) 
         return err_file_errno(file, errno);
	close(fd);
	return 0;
}

static int handle_append_redir(const char *file) 
{
	int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0) 
        return err_file_errno(file, errno);
	close(fd);
	return 0;
}


static int handle_heredoc_redir(t_redir *redir)
{
    int saved;
    
    if (!redir)
         return err_msg("heredoc: internal error"); 
    if (redir->fd < 0) 
        return err_msg("heredoc: not prepared");
    if (close(redir->fd) < 0) 
    {
        saved = errno;        
        redir->fd = -1;          
        return err_file_errno("close", saved);
    }
    redir->fd = -1;
    return 0;
}