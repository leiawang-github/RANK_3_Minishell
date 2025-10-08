/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_external.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 23:11:22 by leiwang           #+#    #+#             */
/*   Updated: 2025/09/17 17:07:39 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

/* Function declarations */
static char *search_in_path(const char *cmd, t_env *env_list);
static int check_executable(const char *path, int verbose);

/*
逻辑：单节点命令的最后一种形式，形如"hey" 或者 "ls -l" 或者 ls -a >out.md"之类的
就是排除了单节点只有重定向，单节点内建命令、无效argv的情况后剩下的情况
实现的核心功能
主函数 exec_single_external()：
调用 which_path() 解析命令路径
处理命令未找到的错误情况
执行外部命令并处理执行失败
正确的内存管理和错误处理
路径解析函数 which_path()：
处理包含 / 的绝对/相对路径
处理不包含 / 的命令名（PATH搜索）
调用 check_executable() 验证可执行性
文件可执行性检查 check_executable()：
使用 access() 检查文件存在性
使用 stat() 检查是否为目录
使用 access() 检查执行权限
支持详细的错误报告
PATH环境变量搜索 search_in_path()：
解析 PATH 环境变量
逐个目录拼接命令名
检查每个候选路径的可执行性
字符串分割函数 ft_strtok()：
线程安全的字符串分割实现
类似于 strtok_r 的功能
用于解析 PATH 环境变量
*/

int exec_single_external(t_mini *pipeline, t_env *env_list, char **envp)
{
    char *path;
    pid_t pid;
    int status; //：存储子进程退出状态的变量地址
    char **current_envp; // 使用当前环境变量
    
    // 将t_env转换为char**数组
    current_envp = env_to_array(env_list);
    if (!current_envp)
        current_envp = envp; // 如果转换失败，使用原始envp
    
    path = which_path(pipeline->cmd_argv[0], env_list); 
    if (!path) 
    {
        err_msg(pipeline->cmd_argv[0], ": command not found", ERR_CMD_NOT_FOUND);
        if (current_envp != envp)
            free_env_array(current_envp);
        return -1;
    }
    pid = fork();
    if (pid == 0) //  child process
    {
        execve(path, pipeline->cmd_argv, current_envp);
        ft_errno(pipeline->cmd_argv[0], errno, ERR_CANNOT_EXEC);
        exit(126); // execve failed
    } 
    else if (pid > 0) // parent process
    {
        while (waitpid(pid, &status, 0) == -1)
            if (errno != EINTR) break;
        free(path);
        if (current_envp != envp)
            free_env_array(current_envp);
        g_last_status = WEXITSTATUS(status);  // 设置全局状态
        return WEXITSTATUS(status);
    } 
    else
    {
        free(path);
        if (current_envp != envp)
            free_env_array(current_envp);
        ft_errno("fork", errno, ERR_SYS_BUILTIN);
        return -1;
    }
}


char *which_path(char *cmd_name, t_env *env_list)  // return value itself is the path to be executed
{
	char *path;
	
	if (!cmd_name || !*cmd_name)
        return NULL;
	if(ft_strchr(cmd_name, '/')) // contains '/' -> absolute or relative path
    { 
        if (check_executable(cmd_name, 1) == 0) // 在知道是绝对路径或者相对路径前提下，
            return ft_strdup(cmd_name); // 直接返回副本, 
        return NULL;
    }
    path = search_in_path(cmd_name, env_list); 
	if(path)
        return path; // 找到返回路径字符串，否则 NULL
	return (NULL);
}


static char *get_env_value(t_env *env_list, const char *name)
{
    while (env_list) 
	{
        if (ft_strncmp(env_list->name, name, ft_strlen(name) + 1) == 0)
            return (env_list->value);    /* 只读返回 */
        env_list = env_list->next;
    }
    return NULL;
}

static char *search_in_path(const char *cmd, t_env *env_list) //should return whatever path, relative or absolute
{
    char   *path_env;
    char   **path_dirs;
    char   *candidate;
    size_t  len_dir, len_cmd;
    int     i;

    if (!cmd || !*cmd)
        return NULL;
    path_env = get_env_value(env_list, "PATH");
    if (!path_env || !*path_env)
        return NULL;
    path_dirs = ft_split(path_env, ':');
    if (!path_dirs)
        return NULL;
    len_cmd = ft_strlen(cmd);
    i = 0;
    while (path_dirs[i]) {
        const char *dir;
        if (*path_dirs[i] == '\0')
            dir = ".";  /* 空段表示当前目录 */
        else
            dir = path_dirs[i];
        len_dir = ft_strlen(dir);
        candidate = malloc(len_dir + 1 + len_cmd + 1);
        if (!candidate)
        {
            i = 0;
            while (path_dirs[i]) {
                free(path_dirs[i]);
                i++;
            }
            free(path_dirs);
            return NULL; 
        }
        ft_memcpy(candidate, dir, len_dir);
        candidate[len_dir] = '/';
        ft_memcpy(candidate + len_dir + 1, cmd, len_cmd);
        candidate[len_dir + 1 + len_cmd] = '\0';

        if (check_executable(candidate, 0) == 0) 
        {
            i = 0;
            while (path_dirs[i]) {
                free(path_dirs[i]);
                i++;
            }
            free(path_dirs);
            return candidate;  /* 命中：返回可执行路径（调用方负责 free） */
        }
        free(candidate);
        i++;
    }
    i = 0;
    while (path_dirs[i]) {
        free(path_dirs[i]);
        i++;
    }
    free(path_dirs);
    return NULL;
}

static int check_executable(const char *path, int verbose) 
{
    struct stat st;
    
    if (!path || !*path)
        return -1;
    if (access(path, F_OK) != 0) /* 检查文件是否存在 */
        return -1;
    if (stat(path, &st) != 0)   /* 检查是否为目录 */
        return -1;
    if (S_ISDIR(st.st_mode))
    {
        if (verbose)
            err_msg(path, ": is a directory", ERR_CANNOT_EXEC);
        return -1;
    }
    if (access(path, X_OK) != 0)   /* 检查是否有执行权限 */
    {
        if (verbose)
            err_msg(path, ": Permission denied", ERR_CANNOT_EXEC);
        return -1;
    }
    return 0;
}

/*
 * 线程安全的字符串分割函数，类似于 strtok_r
 * str: 要分割的字符串
 * delim: 分隔符
 * saveptr: 保存状态的指针
 */



/*
 * 检查文件是否可执行
 * 返回值: 0 表示可执行, -1 表示不可执行
 * 如果 verbose=1，会在检查失败时直接打印错误消息
#include <unistd.h>
int access(const char *pathname, int mode);

// mode 参数：
F_OK  // 文件是否存在
R_OK  // 是否有读权限
W_OK  // 是否有写权限  
X_OK  // 是否有执行权限

#include <sys/stat.h>
int stat(const char *pathname, struct stat *statbuf);

// 检查文件类型：
S_ISREG(st.st_mode)  // 是普通文件
S_ISDIR(st.st_mode)  // 是目录
S_ISLNK(st.st_mode)  // 是符号链接

#include <unistd.h>
int execve(const char *pathname, char *const argv[], char *const envp[]);

// execve 成功时不会返回（进程被替换）
// execve 失败时返回 -1，并设置 errno
*/