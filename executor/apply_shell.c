#include "minishell.h"


void add_shell_var(t_var **head, const char *key, const char *value)
{
    t_var *cur = *head;

    while (cur)
    {
        if (ft_strcmp(cur->key, key) == 0)
        {
            free(cur->value);
            cur->value = ft_strdup(value);
            return ;
        }
        cur = cur->next;
    }

    t_var *new = malloc(sizeof(t_var));
    if (!new) return ;
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->next = NULL;

    if (!*head)
        *head = new;
    else
    {
        cur = *head;
        while (cur->next)
            cur = cur->next;
        cur->next = new;
    }
}
