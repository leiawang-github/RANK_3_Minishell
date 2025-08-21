#include "minishell.h"

// count how many nodes in the list

// int count_cmds(t_cmd *head)
// {
//     int count = 0;
//     t_cmd *cur = head;

//     while (cur)
//     {
//         count++;
//         cur = cur->next;
//     }
//     return (count);
// }

int	executor(t_cmd *head)
{
	if (!head)
		return (0);
	if (head->next == NULL)
	{
		return (exec_single(head));
	}
	else
	{
		return (exec_pipeline(head));
	}
}




