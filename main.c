/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 00:30:37 by rtiberio          #+#    #+#             */
/*   Updated: 2025/09/26 12:59:03 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

int g_last_status = 0;

int	status_code(int arg)
{
	static int	code;

	if (arg >= 0 && arg <= 255)
		code = arg;
	return (code);
} 
//by default the static variable iniciates in 0.

void	error_input(char *input, char *msg)
{
	if (input)
		free (input);
	if (msg)
		printf("%s\n", msg);
}
 //TODO corregir bien status code y el valor de lo que devuelve lexer!
int handle_input(t_shell *shell, t_token **tokens)
{
	char	*input;
	int		r;

	r = 0;
	input = readline("minishell$ ");
	if (!input)
	{
		error_input(NULL, "exit");
		status_code(1);
		return (-1);
	}
	if (*input)
	{
		add_history(input);
		*tokens = lexer(input);
		free (input);
		if (!*tokens)
			return (1);
	}
	else
		*tokens = NULL;
	if (*tokens)
		r = check_vars_shell(shell, *tokens);
	status_code(0);
	return (r);
}

int	main(int ac, char **av, char **env)
{
	t_shell	*shell;
	t_token	*tokens;
	t_mini	*mini;
	int		status;

	if (ac != 1)
		return (1);
	shell = iniciate_t_shell(env);
	if (!shell)
		return (1);
	while (1)
	{
		status = handle_input(shell, &tokens);
		if (status == -1)
			break ;
		if (status == 1)
			continue ;
		mini = malloc(sizeof(t_mini));
		if (!mini)
		{
			token_lstclear(&tokens, &free_null);
			// error_input(NULL, "malloc error");
			break ;
		}
		// mini = NULL; // 这行代码没有意义，已删除
		status = parser(shell, tokens, &mini);
		if (status == -1)
		{
			token_lstclear(&tokens, &free_null);
			mini_lstclear(&mini, &free_null);
			break ;
		}
		if (status == 1)
			continue ;
		//execute
		status = execute_command(mini, shell->envp, shell->env_list);
		if (status != 0)
		{
			mini_lstclear(&mini, &free_null);
			break;
		}
		mini_lstclear(&mini, &free_null);
	}
	clear_history(); //! Ver que onda esto porque en el subject es rl_clear_history()
	shell_clear(&shell, &free_null);
	(void)av;
	return (0);
}

// int	main(int ac, char **av, char **env)
// {
// 	t_env	*env_list;
// 	char	*input;
// 	t_token	*tokens;

// 	if (ac != 1)
// 		return (1);
// 	env_list = env_handling(env);
// 	if (!env_list)
// 		return (1);
// 	printf("env_list ok\n");
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			rl_clear_history();
// 			break ;
// 		}
// 		if (ft_strcmp(input, "exit") == 0)
// 		{
// 			printf("exit\n");
// 			free(input);
// 			rl_clear_history();
// 			break ;
// 		}
// 		if (*input)
// 			add_history(input);
// 		printf("input = %s\n", input);
// 		if (*input)
// 		 	printf("input ok\nAhora viene parsing\n");
// 		tokens = lexer(input);
// 		if (!tokens)
// 		{
// 			free(input);
// 			printf("error parsing\n");
// 			rl_clear_history();
// 			break ;
// 		}
// 		free (input);
// 	}
// 	env_lstclear(&env_list, &free_null);
// 	(void)av;
// 	return (0);
// }
