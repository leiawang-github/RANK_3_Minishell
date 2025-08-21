#include "minishell.h"

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *head = NULL;
    t_cmd *tail = NULL;

    while (tokens)
	{
        t_cmd *node = malloc(sizeof(t_cmd));
        if (!node)
            return NULL;
        init_cmd(node);
        parse_simple_cmd(&tokens, node);
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
    }
    return head;
}


void init_cmd(t_cmd *simple_cmd)
{
    simple_cmd->cmd_argv = NULL;
    simple_cmd->redirs = NULL;
    simple_cmd->assigns = NULL;
    simple_cmd->cmd_type = NULL;
    simple_cmd->is_builtin = 0;
    simple_cmd->next = NULL;
}


// parse_simple_cmd(&tokens, node)
// {

// }
