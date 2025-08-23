#include "minishell.h"

int	executor(t_cmd *head)
{
	if (!head)
		return (0);
	if (head->next == NULL)
	{
		exec_single_cmd(head);
	}
	else
	{
		exec_pipeline(head); // for (cmd = head; cmd != NULL; cmd = cmd->next) {
			// 一律 fork（内建也在子进程跑）
	}
}



