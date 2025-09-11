#ifndef MINISHELL_DEF_H
# define MINISHELL_DEF_H

//# include <stddef.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdbool.h>
# include "libft.h"

//env_copy
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}				t_env;

//lexer
typedef enum e_token_type
{
	END,
	IN,	//<
	OUT, //>
	HEREDOC, //<<
	APPEND, //>>
	PIPE, //|
	WORD, //words, commands, arguments, etc.
}			t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*prev;
	struct s_token		*next;
}				t_token;

//parsing
typedef struct	s_redir
{
    t_token_type	redir_type;   // which kind of redirection
    char			*file; // file path or heredoc delimiter
    char			*delimiter;// this is the char* after remove quotes(either single or double),ex: "EOF" -> EOF
	int				do_expand; // only for heredoc one case: if delimiter is quoted, do_expand = 0;
	int				fd;
	bool			fd_is_open;
    struct s_redir	*next;   // pointer for linking multiple redirs
}				t_redir;

typedef struct	s_mini //! terminar de definir
{
	char			*cmd_name; //para el primer argumento
	t_redir			*redirs;
	char			**cmd_argv;
	char			**assigns; //asignaciones de variables locales como por ejemplo Comando: VAR=value echo $VAR . assigns contendría algo como ["VAR=value"]. cmd_argv contendría ["echo", "$VAR"].
	bool			is_builtin;
	int				argc; //número de argumentos en cmd_argv
	int				num_redirs; //número de redirecciones en redirs
	int				exit_status; //estado de salida del comando
	int				stdin_fd; //para guardar el stdin original
	int				stdout_fd; //para guardar el stdout original
	char			*heredoc_delimiter; //array de strings para los contenidos de los heredocs //! ya esta en refirs?
	struct s_mini	*next;
}				t_mini;

//execute


#endif

//-------------------------------what I need possibly from parser-----------------------
//pipeline_list
/*
typedef struct s_cmd 
{
    char     **cmd_argv; // first one will be cmd_name, the comes with arguments
    t_redir   *redirs; // a linked list, see below
    struct s_cmd *next;
} t_cmd;

typedef struct s_redir {
    t_token_type redir_type;   // which kind of redirection
    char        *target; // file path or heredoc delimiter
    int          do_expand; // only for heredoc
    struct s_redir *next;   // pointer for linking multiple redirs
}   t_redir; */

// typedef enum e_redir_type{
//     R_REDIR_IN,
//     R_REDIR_OUT,
//     R_REDIR_APPEND,
//     R_REDIR_HEREDOC
// } t_redir_type;
//---------------------------------------------------------------------------------------