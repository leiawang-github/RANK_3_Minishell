# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/02 18:47:43 by rtiberio          #+#    #+#              #
#    Updated: 2025/09/26 13:02:34 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MY_SOURCES	=	main.c\
				t_shell/env_handling.c\
				t_shell/env_lists.c\
				t_shell/env_shlvl.c\
				t_shell/t_shell_handling.c\
				t_shell/env_to_array.c\
				exit_functions/free_mem.c\
				lexer/copy_tokens.c\
				lexer/count_tokens_utils.c\
				lexer/lexer_input.c\
				lexer/syntax_check.c\
				lexer/t_token_edit_list.c\
				lexer/tokenize.c\
				parser/parsing_input.c\
				parser/variable_expansion_utils.c\
				parser/variable_expansion_check.c\
				parser/variable_expansion.c\
				executor/analyze_cmd.c\
				executor/builtin_cd.c\
				executor/builtin_env.c\
				executor/builtin_export.c\
				executor/builtin_implementation.c\
				executor/builtin_unset.c\
				executor/exec_builtin.c\
				executor/exec_pipeline.c\
				executor/exec_redirs.c\
				executor/exec_single_external.c\
				executor/executor.c\
				executor/ft_error.c\
				executor/preprocess_heredoc.c\
				executor/signal.c\
				utils/array_size.c\
				utils/ft_isspace.c\
				utils/ft_strcmp.c
				
MY_OBJECTS	=	${MY_SOURCES:.c=.o}

MY_HEADERS	=	include/lexer.h\
				include/minishell_def.h\
				include/minishell.h\
				include/parsing.h\
				include/t_shell_handling.h\
				include/var_expansions.h\

NAME		=	minishell

RM			=	rm -fr

GCC			=	cc

CFLAGS		=	-Wall -Wextra -Werror -g -Iinclude -I/usr/local/opt/readline/include

LDFLAGS		=	-L/usr/local/opt/readline/lib -lreadline

LIBFT		=	libft

all:		${NAME}

.c.o:
			${GCC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${MY_OBJECTS}
			@make -C ${LIBFT}
			${GCC} ${CFLAGS} -o ${NAME} ${MY_OBJECTS} -L ${LIBFT} -lft ${LDFLAGS}

clean:
			${RM} ${MY_OBJECTS}
			@make clean -C ${LIBFT}

fclean:		clean
			${RM} ${NAME} ${LIBFT}/libft.a
			@make fclean -C ${LIBFT}

re:			fclean all

.PHONY:		all clean fclean re