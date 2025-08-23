#include "minishell.h"

int exec_single_cmd(t_cmd *head)
{
	if (is_builtin(cmd->argv[0]))
	{
		backup_fds();
		if (apply_redirs_in_parent(cmd->redirs) < 0) {
			restore_fds();
			return 1;   // redir 错误，退出码 1
		}
		int rc = run_builtin(cmd->argv);  // 内建函数自己返回退出码
		restore_fds();
		return rc;   // ← 返回值就是内建命令执行的退出码
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
    		if (apply_redirs_in_child(cmd->redirs) < 0)
        		_exit(1);
   			execve(path, argv, envp);
    		_exit((errno == ENOENT) ? 127 : 126);
		}
		waitpid(pid, &status, 0);
		return WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
	}
}
