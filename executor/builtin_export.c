/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:20:10 by leia              #+#    #+#             */
/*   Updated: 2025/09/15 23:00:24 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"

/* Function declarations */
static int show_all_exported_vars(t_env *env_list);
static t_env **convert_list_to_array(t_env *env_list);
static void sort_env_array(t_env **nodes, int count);
static int process_export_arg(const char *arg, t_env **env_list);
static int validate_and_set_var(const char *arg, t_env **env_list);
static t_env *find_env_var(t_env *env_list, const char *name);
static void add_or_update_env_var(t_env **env_list, const char *name, const char *value);

/*
参数处理：区分 VAR=value 和 VAR
变量名验证：检查标识符有效性
环境变量操作：设置/更新变量
显示功能：排序输出所有变量
静默跳过选项：遇到 - 开头的参数就跳过

打印链表：
- 把链表convert成char **；
- 重新排列数组
- 打印数组
*/

int builtin_export(char **argv, t_env **env_list)
{
    int argc = 0;
    int result = 0;
    int i = 1;

    while (argv[argc])
        argc++;

    if (argc == 1)
        return show_all_exported_vars(*env_list);
    while (i < argc) {
        if (argv[i][0] == '-' && argv[i][1] != '\0')
        {
            i++;
            continue;  // 不是 return，而是 continue
        }
        if (process_export_arg(argv[i], env_list) != 0)
            result = 1;
        i++;
    }
    return result;
}

static int show_all_exported_vars(t_env *env_list)
{
    t_env **nodes = convert_list_to_array(env_list);
    if (!nodes) return 1;

    int count;

    count = 0;
    while (nodes[count]) count++;
    sort_env_array(nodes, count);

    // 输出
    int i = 0;
    while (i < count) {
        printf("declare -x %s", nodes[i]->name);
        if (nodes[i]->value && nodes[i]->value[0]) {
            printf("=\"%s\"", nodes[i]->value);
        } else {
            printf("=\"\"");
        }
        printf("\n");
        i++;
    }

    free(nodes);
    return 0;
}

static t_env **convert_list_to_array(t_env *env_list)
{
    t_env *current;
    int count;

    current = env_list;
    count = 0;
    while (current)
    {
        count++;
        current = current->next;
    }
    if (count == 0)
        return NULL;
    t_env **array = malloc((count + 1) * sizeof(t_env*));
    if (!array)
        return NULL;
    current = env_list;
    int i = 0;
    while (current)
    {
        array[i] = current;  // 存储节点指针
        current = current->next;
        i++;
    }
    array[count] = NULL;  // 数组末尾设为 NULL
    return array;
}

static void sort_env_array(t_env **nodes, int count)
{
    int i, j;
    t_env *temp;

    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (ft_strcmp(nodes[j]->name, nodes[j + 1]->name) > 0)
            {
                temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

static int process_export_arg(const char *arg, t_env **env_list)
{
    const char *equal_pos;
    
    if (!arg || !*arg)
        return 0;
    
    equal_pos = ft_strchr(arg, '=');
    if (equal_pos)
    {
        // VAR=value 格式
        return validate_and_set_var(arg, env_list);
    }
    else
    {
        // VAR 格式 - 只是标记为exported，不改变值
        if (!is_valid_var_name(arg))
        {
            err_msg("export", ": not a valid identifier", ERR_SYS_BUILTIN);
            return 1;
        }
        // 如果变量不存在，创建它
        if (!find_env_var(*env_list, arg))
        {
            add_or_update_env_var(env_list, arg, "");
        }
        return 0;
    }
}

static int validate_and_set_var(const char *arg, t_env **env_list)
{
    const char *equal_pos;
    char *name;
    char *value;
    int name_len;
    
    equal_pos = ft_strchr(arg, '=');
    if (!equal_pos)
        return 1;
    
    name_len = equal_pos - arg;
    name = ft_substr(arg, 0, name_len);
    if (!name)
        return 1;
    
    if (!is_valid_var_name(name))
    {
        free(name);
        err_msg("export", ": not a valid identifier", ERR_SYS_BUILTIN);
        return 1;
    }
    
    value = ft_strdup(equal_pos + 1);
    if (!value)
    {
        free(name);
        return 1;
    }
    
    add_or_update_env_var(env_list, name, value);
    free(name);
    free(value);
    return 0;
}

static t_env *find_env_var(t_env *env_list, const char *name)
{
    while (env_list)
    {
        if (ft_strcmp(env_list->name, name) == 0)
            return env_list;
        env_list = env_list->next;
    }
    return NULL;
}

static void add_or_update_env_var(t_env **env_list, const char *name, const char *value)
{
    t_env *existing;
    
    existing = find_env_var(*env_list, name);
    if (existing)
    {
        // 更新现有变量
        free(existing->value);
        existing->value = ft_strdup(value);
    }
    else
    {
        // 添加新变量
        t_env *new_var = env_lstnew(ft_strdup(name), ft_strdup(value));
        if (new_var)
            env_lstadd_back(env_list, new_var);
    }
}
