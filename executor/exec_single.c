//when parsing only returns a single node
// dive into the node, see what's the element of the struct
//based on cmd_type, we apply different policies


#include "minishell.h"

static int exec_single(t_cmd *cmd)
{
    int bak[2];
    pid_t pid;
    int   st;

    if (cmd->type == CMD_EMPTY)
        return (0);

    if (cmd->type == CMD_ASSIGN_ONLY)
    {
        apply_assigns_to_shell(cmd->assigns);
        return (0);
    }
    else if (cmd->type == CMD_REDIR_ONLY)
    {
        if (backup_fds(bak) < 0)
            return (1);
        if (apply_redirs_in_parent(cmd->redirs) < 0)
        {
            restore_fds(bak);
            return (1);
        }
        restore_fds(bak);
        return (0);
    }
    else if (cmd->type == CMD_ASSIGN_AND_REDIR)
    {
        if (backup_fds(bak) < 0)
            return (1);
        if (apply_redirs_in_parent(cmd->redirs) < 0)
        {
            restore_fds(bak);
            return (1);
        }
        apply_assigns_to_shell(cmd->assigns);
        restore_fds(bak);
        return (0);
    }

	//CMD_WITH_ARGV
    if (cmd->cmd_argv && cmd->cmd_argv[0] && is_builtin(cmd->cmd_argv[0]))
    {
        if (backup_fds(bak) < 0)
            return (1);
        if (apply_redirs_in_parent(cmd->redirs) < 0)
        {
            restore_fds(bak);
            return (1);
        }
        if (cmd->assigns)
            apply_assigns_to_shell(cmd->assigns);
        st = run_builtin(cmd->cmd_argv);
        restore_fds(bak);
        return (st);
    }
    pid = fork();
    if (pid < 0)
        return (1);
    if (pid == 0)
    {
        if (apply_redirs_in_child(cmd->redirs) < 0)
            _exit(1);
        if (cmd->assigns)
            apply_assigns_to_child_env(cmd->assigns);
        if (!cmd->cmd_argv || !cmd->cmd_argv[0])
            _exit(0);
        if (is_builtin(cmd->cmd_argv[0]))
            _exit(run_builtin(cmd->cmd_argv));
        execve_with_path(cmd->cmd_argv[0], cmd->cmd_argv, get_envp());
        perror(cmd->cmd_argv[0]);
        _exit(127);
    }
    if (waitpid(pid, &st, 0) < 0)
        return (1);
    return (interpret_status(st));
}
