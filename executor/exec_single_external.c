/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_external.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 23:11:22 by leiwang           #+#    #+#             */
/*   Updated: 2025/09/16 00:36:18 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/env_copy.h"

/*
逻辑：单节点命令的最后一种形式，形如“hey” 或者 “ls -l” 或者 ls -a >out.md"之类的
就是排除了单节点只有重定向，单节点内建命令、无效argv的情况后剩下的情况
1. 解析argv[0], 类似于ls，要在 PATH 环境变量里的目录逐一拼接，找出 /bin/ls 或 /usr/bin/ls 之类的可执行文件。
→ 如果找不到，就要报错：minishell: ls: command not found：这一步的难点在于，如何拼接？
思路：用split函数把char*分成多个字符串再strjoin后和strcmp（外部命令，joined后的str）比较

2、
1. 判断 argv[0]

包含 /（比如 /bin/ls、./a.out、../script.sh）
→ 当作路径处理。

access(path, F_OK)：存在吗？

stat(path)：是不是目录？（目录要报错 "is a directory"，退出码 126）

access(path, X_OK)：有执行权限吗？（没有报 "Permission denied"，退出码 126）

全部通过 → execve(path, argv, envp)

不包含 /（比如 ls、grep、python3）
→ 当作裸命令，需要去 PATH 搜索。

从环境变量里拿到 PATH=/bin:/usr/bin:...

逐个拼接目录 + / + argv[0]，然后按上面相同的 access/stat 检查逻辑。

找到第一个可执行的就 execve()

全部没找到 → minishell: xxx: command not found，退出码 127


*/

int exec_single_external(t_cmd *pipeline, t_env *env_list, char **envp)
{
	char *path;

	path = is_path(pipeline, env_list);
	if (!path)
	{
		// command not found / 错误已在里面处理
		g_last_status = 127;
		return;
	}
	execve(path, pipeline->argv, envp);
}


static char *is_path(char *pipeline, t_env *env) //用来判断一个命令里面有没有/命令等。
{

	if(ft_strchr(pipeline, '/'))// 如果含有/，执行一些命令
    {
        if (check_executable(pipeline) == 0)
            return ft_strdup(pipeline); // 直接返回副本
        else
            return NULL;
    }
    else
        return search_in_path(pipeline, env); // 找到返回路径字符串，否则 NULL
}
