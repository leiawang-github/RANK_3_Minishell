/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:18:11 by leia              #+#    #+#             */
/*   Updated: 2025/10/19 18:47:05 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/minishell_def.h"
#include <readline/readline.h>
#include <readline/history.h>

static struct sigaction g_prev_int;
static struct sigaction g_prev_quit;

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

int install_heredoc_parent_signals(void)
{
    struct sigaction sa;

    if (!g_sig_saved)
    {
        sigaction(SIGINT,  NULL, &g_prev_int);
        sigaction(SIGQUIT, NULL, &g_prev_quit);
        g_sig_saved = 1;
    }
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) < 0)
        return ft_errno("sigaction(SIGINT)", errno, ERR_SYS);
    if (sigaction(SIGQUIT, &sa, NULL) < 0)
        return ft_errno("sigaction(SIGQUIT)", errno, ERR_SYS);
    return 0;
}

int restore_interactive_signals(void)
{
    if (g_sig_saved)
    {
        if (sigaction(SIGINT,  &g_prev_int,  NULL) < 0)
            return ft_errno("sigaction(SIGINT)", errno, ERR_SYS);
        if (sigaction(SIGQUIT, &g_prev_quit, NULL) < 0)
            return ft_errno("sigaction(SIGQUIT)", errno, ERR_SYS);
        g_sig_saved = 0;
    }
    return 0;
}
