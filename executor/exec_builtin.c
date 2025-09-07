/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:35:23 by leia              #+#    #+#             */
/*   Updated: 2025/09/06 06:25:38 by leia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int exec_builtin(pipeline, envp)
{


    
}

static int backup_fds()
{
	int saved_stdin;
	int saved_stdout;
	int saved_stderr;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	return 0;
}

static int restore_fds()
{
	int saved_stdin;
	int saved_stdout;
	int saved_stderr;
	
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
	return 0;
}
