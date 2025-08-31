/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 23:02:10 by leia              #+#    #+#             */
/*   Updated: 2025/08/31 15:33:16 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	executor(t_cmd *pipeline)
{
	if (!pipeline)
		return (0);
	if (pipeline->next == NULL)
	{
		exec_single_cmd(pipeline);
	}
	else
	{
		exec_pipeline(pipeline); // for (cmd = head; cmd != NULL; cmd = cmd->next) {
			// 一律 fork（内建也在子进程跑）
	}
}



