/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:35:05 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 19:30:00 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

// minishell subject : cd with only a relative or absolute path
/*how to achieve
要点（最小实现版）：

1. 只接受一个参数：cd path。多于/少于一参数直接报错返回 1。

2. chdir(path) 失败按 perror 风格报错，返回 1。

3. 成功后尽量更新 PWD/OLDPWD（用 getcwd，失败可忽略对应更新），最后返回 0。

不处理 cd -、无参数跳 HOME、CDPATH、逻辑/物理路径模式等扩展行为。

*/

int mini_cd(char **argv, t_env *env)
{
    char *target_dir;
    char *newpwd;

    if (!argv[1])// only cd + path(either absolute or relative)
    {
        target_dir = get_env_var(env, "HOME");
        if (!target_dir || !*target_dir)
            return err_msg("cd", ": HOME not set", ERR_SYS_BUILTIN);
    }
    else
        target_dir = argv[1]; // set second argv as the path where cd cmd shoulg go
    if (chdir(target_dir) == -1)
          return ft_errno("cd", errno, ERR_SYS_BUILTIN);
    newpwd = getcwd(NULL, 0); //从系统里拿出来的pwd
    if (newpwd && env)
    {
        update_pwd(env, newpwd);
        free(newpwd);
    }
    g_last_status = 0;
    return 0;
}

static char *get_env_var(t_env *env, const char *name) //只针对HOME情况，也就是cd后面没有参数
{
    t_env *current = env;

    while (current)
    {
        if (ft_strcmp(current->name, name) == 0) // 找到匹配的变量名
            return current->value;  // 返回变量值
        current = current->next;  // 移动到下一个节点
    }
    return NULL;  // 没找到，返回NULL
}


static void update_pwd(t_env *env, const char *new_path)
{
    t_env *current = env;

    while (current) {
        if (ft_strcmp(current->name, "PWD") == 0) {
            if (current->value)
                free(current->value);
            current->value = ft_strdup(new_path);
            if (!current->value)
            {
                // 如果malloc失败，至少设置为空字符串
                current->value = ft_strdup("");
            }
            return;
        }
        current = current->next;
    }
}
