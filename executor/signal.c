/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:18:11 by leia              #+#    #+#             */
/*   Updated: 2025/10/24 01:18:30 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell.h"
#include "../include/minishell_def.h"

void	set_parent_ignore_signals(void (**old_i)(int), void (**old_q)(int))
{
	*old_i = signal(SIGINT, SIG_IGN);
	*old_q = signal(SIGQUIT, SIG_IGN);
}

int	fail_wait_cleanup(int *pfd, void (*old_i)(int), void (*old_q)(int))
{
	restore_parent_signals(old_i, old_q);
	close(pfd[0]);
	return (g_last_status = 1);
}

void	restore_parent_signals(void (*old_i)(int), void (*old_q)(int))
{
	signal(SIGINT, old_i);
	signal(SIGQUIT, old_q);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
