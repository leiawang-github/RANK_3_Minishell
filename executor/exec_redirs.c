/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_only_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:27:17 by leia              #+#    #+#             */
/*   Updated: 2025/09/01 23:43:46 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int  cmd_contains_only_redirs(t_cmd *cmd) //在父进程应用重定向
{
	t_redir *redir = cmd->redirs;

	while (redir)
	{
		int fd;
		if (redir->redir_type == R_REDIR_IN) // < 语义：把文件内容作为命令的标准输入。
			redir->fd = handle_input_redir(redir->file);
		else if (redir->redir_type == R_REDIR_OUT) // > 语义：把命令的标准输出写入文件，覆盖原有内容。
			redir->fd = handle_input_redir(redir->file);
		else if (redir->redir_type == R_REDIR_APPEND) // >> 语义：把命令的标准输出写入文件，追加到文件末尾。
			redir->fd = handle_append_redir(redir->file);
		else if (redir->redir_type == R_REDIR_HEREDOC) // <<
			 redir->fd = handle_heredoc_redirect(redir->delimiter);

		if (redir->fd == -1)
            return (-1);
        redir = redir->next;
	}
	apply_redirs(cmd);
	return (0);
}

static int handle_input_redirect(char *file)
{
    int fd;
    
    fd = open(file, O_RDONLY);
    if (fd < 0) {
        ft_perror(file);
        return (-1);
    }
    
    return (fd);
}

static int handle_output_redirect(char *file) 
{
    int fd;
    
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        ft_perror(file);
        return (-1);
    }
    
    return (fd);
}

static int handle_append_redirect(char *file)
{
    int fd;
    
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        ft_perror(file);
        return (-1);
    }
    return (fd);
}

// int apply_redir_heredoc(t_redir *redir) 
// {
//     if (!redir || redir->heredoc_fd < 0) {
//         ft_perror(NULL);
//         return -1;
//     }
//     if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0) {
//         ft_perror(NULL);
//         close(redir->heredoc_fd);
//         redir->heredoc_fd = -1;
//         return -1;
//     }
//     close(redir->heredoc_fd);
//     redir->heredoc_fd = -1;
//     return 0;
// }

static int handle_heredoc_redirect(char *delimiter) 
{
    int fd[2];
    
    if (pipe(fd) < 0) {
        ft_perror("pipe");
        return (-1);
    }
    create_heredoc_content(fd[WRITE_END], delimiter);
    close(fd[WRITE_END]);
    
    return (fd[READ_END]);
}


void apply_redirs(t_cmd *pipeline) {
    t_redir *redir = pipeline->redirs;
    
    while (redir) {
        if (redir->redir_type == R_REDIR_IN || redir->redir_type == R_REDIR_HEREDOC) {
            if (dup2(redir->fd, STDIN_FILENO) == -1)
                ft_perror(DUPERR, NULL, 1);
        }
        else if (redir->redir_type == R_REDIR_OUT || redir->redir_type == R_REDIR_APPEND) {
            if (dup2(redir->fd, STDOUT_FILENO) == -1)
                ft_perror(DUPERR, NULL, 1);
        }
        
        if (redir->fd > 2)
            close(redir->fd);
        
        redir = redir->next;
    }
}