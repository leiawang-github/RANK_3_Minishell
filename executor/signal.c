/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:18:11 by leia              #+#    #+#             */
/*   Updated: 2025/09/07 17:57:03 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include <readline/readline.h>
#include <readline/history.h>

void sigint_handler(int sig)
{
    (void)sig;
    g_last_status = 130;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigquit_handler(int sig)
{
    (void)sig;
    g_last_status = 131;
    write(1, "Quit: 3\n", 8);
}

void setup_signal_handlers(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
}

void restore_signal_handlers(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}