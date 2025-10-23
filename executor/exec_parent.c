/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:35:23 by leia              #+#    #+#             */
/*   Updated: 2025/10/24 01:56:13 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

/*
[exec_single_builtin_parent(cmd, env_list)]
        |
        v
[保存父进程标准FD (dup 0/1/2)]
        |
        v
[按顺序应用重定向 (<, >, >>, <<)]
        |
        v
[重定向是否出错?] --yes--> [打印错误 + g_last_status=1 -> 跳到恢复FD]
        |
       no
        |
        v
[运行内建命令于父进程 (cd/export/unset/exit)]
        |
        v
[是否为 exit?] --yes--> [清理资源(含历史/内存等) -> 直接 exit(状态码)]
        |
       no
        |
        v
[将返回值写入 g_last_status]
        |
        v
[恢复父进程FD / 关闭临时FD (含 heredoc_fd)]
        |
        v
[return g_last_status]

它是为了执行那些必须在父进程执行的内建命令，并让 shell 的状态与 bash 一致。

换句话说：

“模拟 bash 在执行 cd、export、unset、exit 时的行为，
同时正确地处理重定向，并让 shell 环境（工作目录、变量表）保持更新。”

最终目标 = 让这三件事成立：

✅ 命令效果生效

cd → 改变当前进程的工作目录

export/unset → 修改父进程的环境链表

exit → 退出整个 shell
（这些操作如果放在子进程中，就失效）

✅ 重定向效果只影响本命令，不影响后续 shell

依赖于前面保存 / 恢复的 FD。

✅ 正确返回状态码

g_last_status 被设置为执行结果（0 = success，非 0 = fail）。

这个值最终会：

用于 $?；

被顶层 execute() 返回；

在下一次命令执行或条件判断时使用。

对内置命令（例如 echo, cd, export）来说，不会调用 execve()，而是在 shell 进程内直接执行函数。
所以在这种情况下：

shell 必须在执行 builtin 函数前就完成重定向；

否则 builtin 的输出还会写到原来的 stdout，而非文件。

*/

int	exec_single_builtin_parent(t_mini *cmd, t_env *env_list)
{
	int	parent_fds[3];
	int	ret;

	if (backup_parent_fds(parent_fds) != 0)
	{
		g_last_status = 1;
		return (1);
	}
	if (apply_all_redirs(cmd->redirs) != 0)
	{
		restore_parent_fds(parent_fds);
		g_last_status = 1;
		return (1);
	}
	ret = run_builtin_in_parent(cmd->argv, env_list);
	g_last_status = ret;
	if (g_last_status < 0)
		g_last_status = 1;
	if (g_last_status > 255)
		g_last_status = g_last_status % 256;
	restore_parent_fds(parent_fds);
	return (g_last_status);
}

static int	backup_parent_fds(int parent_fds[3])
{
	int i;

	parent_fds[0] = dup(STDIN_FILENO);
	parent_fds[1] = dup(STDOUT_FILENO);
	parent_fds[2] = dup(STDERR_FILENO);
	if (parent_fds[0] < 0 || parent_fds[1] < 0 || parent_fds[2] < 0)
	{
		i = 0;
		while (i < 3)
		{
			if (parent_fds[i] >= 0)
			{
				close(parent_fds[i]);
				parent_fds[i] = -1;
			}
			i++;
		}
		return (-1);
	}
	return (0);
}

int	restore_parent_fds(int parent_fds[3])
{
	int i;
	int rc;

	rc = 0;
	i = 0;
	while (i < 3)
	{
		if (parent_fds[i] >= 0)
		{
			if (dup2(parent_fds[i], i) < 0)
				rc = -1;
			close(parent_fds[i]);
			parent_fds[i] = -1;
		}
		i++;
	}
	return (rc);
}


