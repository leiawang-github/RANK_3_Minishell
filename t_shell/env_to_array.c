/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+            */
/*   Created: 2025/10/06 22:40:00 by leia              #+#    #+#              */
/*   Updated: 2025/10/06 22:40:00 by leia             ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **env_to_array(t_env *env_list)
{
    t_env *current;
    char **envp;
    int count;
    int i;
    
    if (!env_list)
        return NULL;
    
    // 计算环境变量数量
    current = env_list;
    count = 0;
    while (current)
    {
        count++;
        current = current->next;
    }
    
    // 分配内存：count个字符串指针 + 1个NULL终止符
    envp = malloc(sizeof(char*) * (count + 1));
    if (!envp)
        return NULL;
    
    // 填充数组
    current = env_list;
    i = 0;
    while (current)
    {
        // 格式：NAME=value
        envp[i] = malloc(ft_strlen(current->name) + ft_strlen(current->value) + 2);
        if (!envp[i])
        {
            // 清理已分配的内存
            while (i > 0)
            {
                i--;
                free(envp[i]);
            }
            free(envp);
            return NULL;
        }
        ft_strlcpy(envp[i], current->name, ft_strlen(current->name) + 1);
        ft_strlcat(envp[i], "=", ft_strlen(current->name) + 2);
        ft_strlcat(envp[i], current->value, ft_strlen(current->name) + ft_strlen(current->value) + 2);
        
        current = current->next;
        i++;
    }
    envp[count] = NULL; // 数组末尾设为NULL
    
    return envp;
}

void free_env_array(char **envp)
{
    int i;
    
    if (!envp)
        return;
    
    i = 0;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}
