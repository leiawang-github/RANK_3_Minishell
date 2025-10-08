/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:35:23 by leia              #+#    #+#             */
/*   Updated: 2025/09/15 23:02:47 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

/* Static function declarations */
static int builtin_implementation(t_mini *pipeline, t_env *env_list);
static int apply_redirs_parent(t_redir *r);
static int backup_fds(int bak[3]);
static int restore_fds(int bak[3]);

int exec_builtin_in_single_cmd(t_mini *pipeline, t_env *env_list) //已经知道是单节点builtin, no need to fork
{
    t_mini *node;
    int rc; // return code as side effect
    int bak[3];

    node = pipeline;
    if(node->redirs == NULL) // single node builtin but no redir
    {
        rc = builtin_implementation(pipeline, env_list);
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
        rc = builtin_implementation(pipeline, env_list);
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
        if (r->redir_type == IN)
        {
            fd = open(r->file, O_RDONLY);
            if (fd < 0) return ft_errno(r->file, errno, ERR_SYS_BUILTIN);
            if (dup2(fd, STDIN_FILENO) < 0) { close(fd); return ft_errno("dup2", errno, ERR_SYS_BUILTIN); }
            close(fd);
        }
        else if (r->redir_type == OUT)
        {
            fd = open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) return ft_errno(r->file, errno, ERR_SYS_BUILTIN);
            if (dup2(fd, STDOUT_FILENO) < 0) { close(fd); return ft_errno("dup2", errno, ERR_SYS_BUILTIN); }
            close(fd);
        }
        else if (r->redir_type == APPEND)
        {
            fd = open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) return ft_errno(r->file, errno, ERR_SYS_BUILTIN);
            if (dup2(fd, STDOUT_FILENO) < 0) { close(fd); return ft_errno("dup2", errno, ERR_SYS_BUILTIN); }
            close(fd);
        }
        else if (r->redir_type == HEREDOC)
        {
            if (r->fd >= 0)
            {
                if (dup2(r->fd, STDIN_FILENO) < 0) { return ft_errno("dup2", errno, ERR_SYS_BUILTIN); }
                close(r->fd);
                r->fd = -1;
            }
        }
        r = r->next;
    }
    return 0;
}

int builtin_implementation(t_mini *pipeline, t_env *env_list) //single node without redirection
{
    t_mini *node;

    char *cmd_name;
    node = pipeline;
    cmd_name = node->cmd_argv[0];

    if (ft_strcmp(cmd_name, "echo") == 0)
            return builtin_echo(node->cmd_argv);
    else if (ft_strcmp(cmd_name, "cd") == 0)
        return builtin_cd(node->cmd_argv, env_list);
    else if (ft_strcmp(cmd_name, "pwd") == 0)
        return builtin_pwd();
    else if (ft_strcmp(cmd_name, "export") == 0)
        return builtin_export(node->cmd_argv, &env_list);
    else if (ft_strcmp(cmd_name, "unset") == 0)
        return builtin_unset(node->cmd_argv, &env_list);
    else if (ft_strcmp(cmd_name, "env") == 0)
        return builtin_env(node->cmd_argv, env_list);
    else if (ft_strcmp(cmd_name, "exit") == 0)
        return builtin_exit(node->cmd_argv);
    return 127; // Not a recognized builtin

}


static int backup_fds(int bak[3]) //backup stdin, stdout, stderr
{
    bak[0] = dup(STDIN_FILENO);
    bak[1] = dup(STDOUT_FILENO);
    bak[2] = dup(STDERR_FILENO);
    if (bak[0] < 0 || bak[1] < 0 || bak[2] < 0)
        return ft_errno("dup", errno, ERR_SYS_BUILTIN);
    return 0;
}

static int restore_fds(int bak[3])
{
    if (dup2(bak[0], STDIN_FILENO) < 0)  return ft_errno("dup2", errno, ERR_SYS_BUILTIN);
    if (dup2(bak[1], STDOUT_FILENO) < 0) return ft_errno("dup2", errno, ERR_SYS_BUILTIN);
    if (dup2(bak[2], STDERR_FILENO) < 0) return ft_errno("dup2", errno, ERR_SYS_BUILTIN);
    close(bak[0]);
    close(bak[1]);
    close(bak[2]);
    return 0;
}
