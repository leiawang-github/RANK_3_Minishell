/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:20:10 by leia              #+#    #+#             */
/*   Updated: 2025/09/13 15:50:48 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/env_copy.h"

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
        return show_all_exported_vars(env_list);
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

static  char **convert_list_to_array(t_env *env_list)
{
    t_env *current;  
    int count;

    current = env_list;
    count = ft_lstsize(current);
    if (count == 0) 
        return NULL;
    char **array = malloc((count + 1) * sizeof(char*));
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