/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:02:24 by leia              #+#    #+#             */
/*   Updated: 2025/09/01 17:33:13 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

int exec_single(t_cmd *pipeline)
{
    if (!pipeline->argv || !pipeline->argv[0]) //no command, only redirections
	{
        if (pipeline->redirs)
		{
			backup_fds();
            if (apply_redirs_in_parent(pipeline->redirs) < 0) 
			{ 
				restore_fds();
				return 1; 
			}
            restore_fds();
        }
        return 0;
    }
    if (is_builtin(pipeline->argv[0])) //with builtin command and maybe redirections
	{
        if (pipeline->redirs) 
		{
			backup_fds();
            if (apply_redirs_in_parent(pipeline->redirs) < 0) 
			{ 
				restore_fds();
				return 1; 
			}
            int rc = run_builtin(pipeline->argv);
            restore_fds();
            return rc;
        } else {
            return run_builtin(pipeline->argv);
        }
    }
    pid_t pid = fork();
    if (pid < 0) return 1;
    if (pid == 0) {
        if (apply_redirs_in_child(pipeline->redirs) < 0) _exit(1);
        char *path = resolve_command(pipeline->argv[0], environ);
        if (!path) _exit(127);            // command not found
        execve(path, pipeline->argv, environ);
        _exit(errno == ENOENT ? 127 : 126);
    }
    int st; waitpid(pid, &st, 0);
    return interpret_wait_status(st);
}



static int backup_fds()
{
	int saved_stdin;
	int saved_stdout;
	int saved_stderr;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	return 0;
}

static int restore_fds()
{
	int saved_stdin;
	int saved_stdout;
	int saved_stderr;
	
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
	return 0;
}



	

	

