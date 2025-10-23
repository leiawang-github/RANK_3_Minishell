/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 23:20:10 by leia              #+#    #+#             */
/*   Updated: 2025/10/23 23:39:15 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

/* Function declarations */
// static int show_all_exported_vars(t_env *env_list);
// static t_env **convert_list_to_array(t_env *env_list);
// static void sort_env_array(t_env **nodes, int count);
// static int process_export_arg(const char *arg, t_env **env_list);
// static int validate_and_set_var(const char *arg, t_env **env_list);
// static t_env *find_env_var(t_env *env_list, const char *name);
// static void add_or_update_env_var(t_env **env_list, const char *name, const char *value);

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


/*
[mini_export(argv, env_list)]
        |
        v
[argc == 1 ?] --yes--> [show_all_exported_vars(sorted)]
        |                                 |
       no                                 v
        |                         [g_last_status = 0; return 0]
        v
[opt_err = 0, ident_err = 0, i = 1]
        |
        v
[loop: i < argc]
        |
        v
[arg[i] 以 '-' 开头 且 长度>1 ?] --yes--> [print "invalid option"]
        |                                   [opt_err = 1]
       no                                   [i++ ; continue]
        |
        v
[arg 中含 '=' ?] --yes--> [name = before '=', value = after '=']
        |                  [is_valid_identifier(name)?]
       no                  |----no--> [print "not a valid identifier"]
        |                               [ident_err = 1; i++ ; continue]
        v                  |----yes-> [set/update env: name=value]
[仅 NAME 形式]                          [i++ ; continue]
        |
        v
[is_valid_identifier(NAME)?] --no--> [print "not a valid identifier"]
        |                              [ident_err = 1; i++ ; continue]
       yes
        |
        v
[若 env 中不存在则创建 name，value = NULL]  // 仅标记为导出
        |
        v
[i++ ; 继续循环]
        |
        v
[循环结束]
        |
        v
[opt_err == 1 ?] --yes--> [g_last_status = 2; return 2]   // 选项错误优先
        |
       no
        |
        v
[ident_err == 1 ?] --yes--> [g_last_status = 1; return 1] // 非法标识符
        |
       no
        |
        v
[g_last_status = 0; return 0]
*/

int mini_export(char **argv, t_env **env_list)
{
    int ac;
    int i;
    int result;
    int opt_err;

    ac = 0;
    while (argv && argv[ac])
        ac++;
    if (ac == 1)
    {
        g_last_status = show_all_exported_vars(*env_list);
        return (g_last_status);
    }
    i = 1;
    result = 0;
    opt_err = 0;
    while (i < ac)
    {
        if (argv[i][0] == '-' && argv[i][1] != '\0')
        {
            err_msg("export", ": invalid option", ERR_SYNTAX); /* -> status=2 */
            opt_err = 1;
            i++;
            continue;
        }
        if (process_export_arg(argv[i], env_list) != 0)
            result = 1;
        i++;
    }
    if (opt_err)
        g_last_status = 2;
    else if (result)
        g_last_status = 1;
    else
        g_last_status = 0;
    return (g_last_status);
}

static int show_all_exported_vars(t_env *env_list)
{
    t_env **nodes;
    int count;
    int i;

    i = 0;
    nodes = convert_list_to_array(env_list);
    if (!nodes)
        return (0);
    count = 0;
    while (nodes[count])
        count++;
    sort_env_array(nodes, count);
    while (i < count)
    {
        write(1, "declare -x ", 11);
        write(1, nodes[i]->name, ft_strlen(nodes[i]->name));
        if (nodes[i]->value != NULL)
        {
            write(1, "=\"", 2);
            write(1, nodes[i]->value, ft_strlen(nodes[i]->value));
            write(1, "\"", 1);
        }
        write(1, "\n", 1);
        i++;
    }
    free(nodes);
    return (0);
}

static int process_export_arg(const char *arg, t_env **env_list)
{
    const char *eq;

    if (!arg || !*arg)
        return (0);

    eq = ft_strchr(arg, '=');
    if (eq)
        return validate_and_set_var(arg, env_list);
    if (!is_valid_var_name(arg))
    {
        err_msg("export", ": not a valid identifier", ERR_SYS_BUILTIN);
        return (1);
    }
    if (!find_env_var(*env_list, arg))
        add_or_update_env_var(env_list, arg, NULL);
    return (0);
}

static int validate_and_set_var(const char *arg, t_env **env_list)
{
    const char *eq;
    char *name;
    char *value;
    int name_len;
    int rc;

    eq = ft_strchr(arg, '=');
    if (!eq)
        return (1);
    name_len = (int)(eq - arg);
    name = ft_substr(arg, 0, name_len);
    if (!name)
        return (1);
    if (!is_valid_var_name(name))
    {
        free(name);
        err_msg("export", ": not a valid identifier", ERR_SYS_BUILTIN);
        return (1);
    }
    value = ft_strdup(eq + 1);
    if (!value)
    {
        free(name);
        return (1);
    }
    rc = add_or_update_env_var(env_list, name, value);
    free(name);
    free(value);
    return (rc != 0);
}

static int add_or_update_env_var(t_env **env_list, const char *name, const char *value)
{
    t_env *cur;
    t_env *newv;
    char  *dup_name;
    char  *dup_val;

    cur = find_env_var(*env_list, name);
    if (cur)
    {
        if (cur->value)
            free(cur->value);
        if (value)
        {
            dup_val = ft_strdup(value);
            if (!dup_val)
                return (1);
            cur->value = dup_val;
        }
        else
            cur->value = NULL;
        return (0);
    }
    dup_name = ft_strdup(name);
    if (!dup_name)
        return (1);
    if (value)
    {
        dup_val = ft_strdup(value);
        if (!dup_val)
        {
            free(dup_name);
            return (1);
        }
    }
    else
        dup_val = NULL;
    newv = env_lstnew(dup_name, dup_val);
    if (!newv)
    {
        if (dup_name)
            free(dup_name);
        if (dup_val)
            free(dup_val);
        return (1);
    }
    env_lstadd_back(env_list, newv);
    return (0);
}

t_env	**convert_list_to_array(t_env *env_list)
{
	int		count;
	t_env	**array;
	t_env	*cur;
	int		i;

	count = count_env_nodes(env_list);
	array = (t_env **)malloc(sizeof(t_env *) * (count + 1));
	if (!array)
		return (NULL);
	cur = env_list;
	i = 0;
	while (cur)
	{
		array[i] = cur;
		cur = cur->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

static void	ft_swap(t_env **a, t_env **b)
{
	t_env *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort_env_array(t_env **nodes, int count)
{
	int i;
	int j;

	if (!nodes)
		return ;
	if (count <= 1)
		return ;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if ((nodes[j]->name == NULL && nodes[j + 1]->name != NULL)
				|| (nodes[j]->name && nodes[j + 1]->name
					&& ft_strcmp(nodes[j]->name, nodes[j + 1]->name) > 0))
				ft_swap(&nodes[j], &nodes[j + 1]);
			j++;
		}
		i++;
	}
}
