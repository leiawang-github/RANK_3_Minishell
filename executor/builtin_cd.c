/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:35:05 by leia              #+#    #+#             */
/*   Updated: 2025/09/15 20:24:45 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/env_copy.h"

// cd 不带参数怎么运行的？——就是不管在哪里，只要cd一下，就会回到主目录HOME：所以关键点就是利用chdir函数把cd后的路径
//设置成在env列表中的HOME作为key值，value 作为 cd前往的路径

//需要在父进程改变而不是fork到子进程
//
// 需要实现：cd 到主目录或者绝对/相对路线
//

int builtin_cd(char **argv, t_env *env)
{
    char *target_dir;
    char *newpwd;
    
    if (!argv[1])// only cd without any argvs, we just use HOME as cd's directory after cd cmd is executed
    {
        target_dir = get_env_var(env, "HOME");
        if (!target_dir || !*target_dir)
            return err_msg("cd: HOME not set");
    } 
    else 
        target_dir = argv[1]; // set second argv as the path where cd cmd shoulg go
    if (chdir(target_dir) == -1) 
          return ft_errno("cd", errno);
    newpwd = getcwd(NULL, 0); //从系统里拿出来的pwd
    if (newpwd && env) 
    {
        update_pwd(env, newpwd); 
        free(newpwd);
    }
    g_last_status = 0;
    return 0;
}

char *get_env_var(t_env *env, const char *name) //只针对HOME情况，也就是cd后面没有参数
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


void update_pwd(t_env *env, const char *new_path)
{
    t_env *current = env;
    
    while (current) {
        if (ft_strcmp(current->name, "PWD") == 0) {
            free(current->value);
            current->value = ft_strdup(new_path);
            return;
        }
        current = current->next;
    }
}