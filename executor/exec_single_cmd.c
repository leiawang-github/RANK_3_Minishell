#include "minishell.h"

// [SingleCommandBuiltin]

// argv[0] 存在 & is_builtin == true
//         │
//         ▼
//    backup_fds()
//         │
//         ├─ apply_redirs_in_parent()
//         │     ├─ open/dup2 按序应用
//         │     └─ 失败 → print error → restore_fds() → return 1
//         │
//         ▼
//    rc = run_builtin(argv)
//         │
//         ├─ builtin == exit ?
//         │     ├─ 无效参数 → print → rc!=0 → 继续 restore → return rc
//         │     └─ 有效 → restore_fds() → cleanup → exit(rc)
//         │
//         ▼
//    restore_fds()
//         │
//         ▼
//    return rc   // 写入 $?



int exec_single_cmd(t_cmd *head)
{
	if(!head || !head->cmd_argv || !head->cmd_argv[0])
		return 0;
	if (is_builtin(head->cmd_argv[0]))
	{
		backup_fds();
		if (apply_redirs_in_parent(head->redirs) < 0) //重定向失败
		{
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

int is_builtin(const char *name)
{
    if (!name)
		return 0;
    return (ft_strcmp(name, "echo")  == 0) ||
           (ft_strcmp(name, "cd")    == 0) ||
           (ft_strcmp(name, "pwd")   == 0) ||
           (ft_strcmp(name, "export")== 0) ||
           (ft_strcmp(name, "unset") == 0) ||
           (ft_strcmp(name, "env")   == 0) ||
           (ft_strcmp(name, "exit")  == 0);
}

int backup_fds()
{
	int saved_stdin;
	int saved_stdout;
	int saved_stderr;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	return 0;
}

int apply_redirs_in_parent(t_cmd *head->redirs) //在父进程应用重定向
{
	t_redir *curr = head->redirs;
	while (curr)
	{
		int fd;
		if (curr->redir_type == R_REDIR_IN) // < 语义：把文件内容作为命令的标准输入。
		{
			fd = open(curr->target, O_RDONLY);
			if (fd < 0)
			{
				ft_perror(curr->target);
				return (-1);
			}
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				ft_perror(curr->target);
				close(fd);
				return (-1);
			}
			close(fd);
		}




		
		else if (curr->redir_type == R_REDIR_OUT) // >
		{
			fd = open(curr->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open");
				return -1;
			}
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return -1;
			}
			close(fd);
		}
		else if (curr->redir_type == R_REDIR_APPEND) // >>
		{
			fd = open(curr->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror("open");
				return -1;
			}
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(fd);
				return -1;
			}
			close(fd);
		}
		else if (curr->redir_type == R_REDIR_HEREDOC) // <<
		{
			int pipefd[2];
			if (pipe(pipefd) < 0)
			{
				perror("pipe");
				return -1;
			}
			pid_t pid = fork();
			if (pid < 0)
			{
				perror("fork");
				close(pipefd[0]);
				close(pipefd[1]);
				return -1;
			}
			else if (pid == 0) // 子进程写入 heredoc 内容
			{
				close(pipefd[0]); // 关闭读端
				char *line;
				while ((line = readline("> ")) != NULL)
				{
					if (ft_strcmp(line, curr->target) == 0)
					{
						free(line);
						break;
					}
					if (curr->do_expand)
						line = expand_variables(line); // 变量展开函数
					write(pipefd[1], line, ft_strlen(line));
					write(pipefd[1], "\n", 1);
					free(line);
				}
		}
}
