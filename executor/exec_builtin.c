/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:35:23 by leia              #+#    #+#             */
/*   Updated: 2025/09/14 11:40:28 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/env_copy.h"

int exec_builtin_in_single_cmd(t_cmd *pipeline) //已经知道是单节点builtin, no need to fork
{
    t_cmd *node;
    int rc; // return code as side effect
    int bak[3];

    node = pipeline;
    if(node->redirs == NULL) // single node builtin but no redir
    {
        rc = builtin_implementation(pipeline);
        g_last_status = rc;
        return rc;
    }
    else
    {
        if (backup_fds(bak) < 0)
        {
            g_last_status = 1;
            return 1;
        }
        if (apply_redirs_parent(node->redirs) < 0)
        {
            restore_fds(bak);
            return -1;
        }
        rc = builtin_implementation(pipeline);
        if (restore_fds(bak) < 0)
        {
            g_last_status = 1;
            return 1;
        }
        g_last_status = rc;
        return rc;
    }
}

static int apply_redirs_parent(t_redir *r)
{
    int fd;
    while (r)
    {
        if (r->redir_type == R_REDIR_IN)
        {
            fd = open(r->file, O_RDONLY);
            if (fd < 0) return ft_perror(r->file);
            if (dup2(fd, STDIN_FILENO) < 0) { close(fd); return ft_perror("dup2"); }
            close(fd);
        }
        else if (r->redir_type == R_REDIR_OUT)
        {
            fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) return ft_perror(r->file);
            if (dup2(fd, STDOUT_FILENO) < 0) { close(fd); return ft_perror("dup2"); }
            close(fd);
        }
        else if (r->redir_type == R_REDIR_APPEND)
        {
            fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) return ft_perror(r->file);
            if (dup2(fd, STDOUT_FILENO) < 0) { close(fd); return ft_perror("dup2"); }
            close(fd);
        }
        else if (r->redir_type == R_REDIR_HEREDOC)
        {
            if (r->fd >= 0)
            {
                if (dup2(r->fd, STDIN_FILENO) < 0) { return ft_perror("dup2"); }
                close(r->fd);
                r->fd = -1;
            }
        }
        r = r->next;
    }
    return 0;
}

int builtin_implementation(t_cmd *pipeline) //single node without redirection
{
    t_cmd *node;

    char *cmd_name;
    node = pipeline;
    cmd_name = node->argv[0];
    
    if (ft_strcmp(cmd_name, "echo") == 0)
            return builtin_echo(node->argv);
    else if (ft_strcmp(cmd_name, "cd") == 0)
        return builtin_cd(node->argv);
    else if (ft_strcmp(cmd_name, "pwd") == 0)
        return builtin_pwd();
    else if (ft_strcmp(cmd_name, "export") == 0)
        return builtin_export(node->argv);
    else if (ft_strcmp(cmd_name, "unset") == 0)
        return builtin_unset(node->argv);
    else if (ft_strcmp(cmd_name, "env") == 0)
        return builtin_env();
    else if (ft_strcmp(cmd_name, "exit") == 0)
        return builtin_exit(node->argv);
    return 127; // Not a recognized builtin
    
}


static int backup_fds(int bak[3]) //backup stdin, stdout, stderr
{
    bak[0] = dup(STDIN_FILENO);
    bak[1] = dup(STDOUT_FILENO);
    bak[2] = dup(STDERR_FILENO);
    if (bak[0] < 0 || bak[1] < 0 || bak[2] < 0)
        return ft_perror("dup");
    return 0;
}

static int restore_fds(int bak[3])
{
    if (dup2(bak[0], STDIN_FILENO) < 0)  return ft_perror("dup2");
    if (dup2(bak[1], STDOUT_FILENO) < 0) return ft_perror("dup2");
    if (dup2(bak[2], STDERR_FILENO) < 0) return ft_perror("dup2");
    close(bak[0]);
    close(bak[1]);
    close(bak[2]);
    return 0;
}