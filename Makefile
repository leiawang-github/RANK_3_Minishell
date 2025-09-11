# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/26 21:11:41 by leiwang           #+#    #+#              #
#    Updated: 2025/06/26 21:11:42 by leiwang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_EXECUTOR_LIB := libexecutor.a

CC      := cc
CFLAGS  := -Wall -Wextra -Werror -std=c11
INCS    := -Iexecutor -I.

EXECUTOR_SRCS := \
    executor/executor.c \
    executor/analyze_cmd.c \
    executor/exec_redirs.c \
    executor/exec_builtin.c \
    executor/exec_core.c \
    executor/ft_error.c \
    executor/preprocess_heredoc.c

EXECUTOR_OBJS := $(EXECUTOR_SRCS:.c=.o)

.PHONY: all clean fclean re executor_lib

all: executor_lib

executor_lib: $(NAME_EXECUTOR_LIB)

$(NAME_EXECUTOR_LIB): $(EXECUTOR_OBJS)
	@ar rcs $@ $^
	@echo "Built $@"

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	@rm -f $(EXECUTOR_OBJS)

fclean: clean
	@rm -f $(NAME_EXECUTOR_LIB)

re: fclean all
