/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_def.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:06:02 by leiwang           #+#    #+#             */
/*   Updated: 2025/10/19 18:06:03 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_DEF_H
# define MINISHELL_DEF_H

//# include <stddef.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>
# include "../libft/libft.h"

//env_copy
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct	s_shell
{
	/* Pipeline execution fields */
	int **pipes;        /* Array of pipes for pipeline communication */
	int node_index;     /* Current node index in pipeline */
	int node_count;     /* Total number of nodes in pipeline */

	/* Environment management */
	t_env *env_list;   /* Environment variable list (exported variables) */
	t_env *vars;       /* Internal variables (not exported) */
	char **envp;       /* Environment variable array for execve */
}				t_shell;

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
	char			*value;
	t_token_type	type;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

typedef struct s_varexp
{
	int				num_subs;
	char			**subs;
	int				*i_src;
	int				len_new_str;
	char			*new_str;
}				t_varexp;

//parsing
typedef enum e_redir_type
{
    IN,       // <
    OUT,      // >
    HEREDOC,     // <<
    APP       // >>
} t_redir_type;

typedef struct	s_redir
{
    t_redir_type	redir_type;   // which kind of redirection
    char			*target; // when we see a redir, what's next to this redir is a redir target, ex: echo >hi.txt, hi.txt is a target
	// below three only for heredoc
	char			*delimiter;// only applies to heredoc after remove the quotes: "EOF" -> EOF
	int				hdoc_expand; // only for heredoc one case: if delimiter is quoted, do_expand = 0; mark here so we will expand $
	int          	heredoc_fd;  // 预处理后填入：<< 的读端 FD，否则为 -1

    struct s_redir	*next;   // pointer for linking multiple redirs
}				t_redir;


typedef struct	s_mini //! terminar de definir
{
	char			*cmd_name; //para el primer argumento
	char			**cmd_argv;
	t_redir			*redirs;
	//char			**assigns; //asignaciones de variables locales como por ejemplo Comando: VAR=value echo $VAR . assigns contendría algo como ["VAR=value"]. cmd_argv contendría ["echo", "$VAR"].
	bool			is_builtin;
	//int				argc; //número de argumentos en cmd_argv
	//int				num_redirs; //número de redirecciones en redirs
	int				exit_status; //estado de salida del comando
	int				stdin_fd; //para guardar el stdin original
	int				stdout_fd; //para guardar el stdout original
	//char			*heredoc_delimiter; //array de strings para los contenidos de los heredocs //! ya esta en refirs?
	struct s_mini	*next;
}				t_mini;



//execute
/* error.h 或 minishell.h */
#ifndef ERROR_H
# define ERROR_H

enum e_err
{
    ERR_NONE,   // 无错误
    ERR_PARSER, // 语法或解析错误
    ERR_EXEC,   // 执行阶段错误（比如 command not found）
    ERR_SYS,    // 系统调用错误（open、pipe、fork 等失败）
    ERR_BUILTIN // 内建命令错误
};

#endif


#endif
