NAME = test_line
SRC = test_line.c
CC = cc

# ✅ 强制指定 GNU Readline 的头文件路径和库路径
INC = -I/opt/homebrew/opt/readline/include
LIB = -L/opt/homebrew/opt/readline/lib -lreadline

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(INC) $(SRC) $(LIB) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re
