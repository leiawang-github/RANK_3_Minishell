/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leiwang <leiwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:11:44 by leiwang           #+#    #+#             */
/*   Updated: 2025/08/17 21:10:59 by leiwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <readline/readline.h>

//token types: 6 types
typedef enum e_token_type {
    T_WORD,
	T_PIPE,
	T_REDIR_IN,
	t_REDIR_OUT,
	T_REDIR_APP,
	T_HEREDOC
} t_token_type;

//qoutes types: no /single/ double, 3 types
typedef enum e_quote_type {
    Q_NONE = 0,
    Q_SQUOTE = 1,
    Q_DQUOTE = 2
} t_quote_type;

typedef struct s_token {
    t_token_type       type;
    char           *lexeme;      // 原样存放（含引号）
    int             has_squote;  // 可选：此词是否被单引号包裹（或包含单引号片段）
    int             has_dquote;
    struct s_token *next;
} t_token;

//cmd_assign ---------- 变量赋值 如：A=1 B="x y" ----------
typedef struct s_assign {
    char *key;            // "A"
    char *val;            // "1"（去引号、已做变量展开后）
    t_quote_type quote_type;        // 赋值右值的引号语义（需要的话）
    struct s_assign *next;
} t_assign;

//cmd_redir ---------- 重定向：<in  >out  >>app  <<heredoc ----------
typedef enum e_redir_type{
    R_REDIR_IN,
    R_REDIR_OUT,
    R_REDIR_APPEND,
    R_REDIR_HEREDOC
} t_redir_type;

//redir strcut
typedef struct s_redir {
    t_redir_type redir_type;   // which kind of redirection
    char        *target; // file path or heredoc delimiter
    int          do_expand; // only for heredoc
    struct s_redir *next;   // pointer for linking multiple redirs
}   t_redir;

//
typedef enum e_cmd_type {
    CMD_ASSIGN_ONLY,
    CMD_REDIR_ONLY,
    CMD_ASSIGN_ADN_REDIR,
    CMD_WITH_ARGV
}   t_cmd_type;


//cmd_node_format
typedef struct s_simple_cmd {
    t_assign  *assigns; //a linked list
    char     **cmd_argv; // first one will be cmd_name, the comes with arguments
    t_redir   *redirs; // a linked list
    t_cmd_type cmd_type;
    int          is_builtin;
    struct s_cmd *next;
} t_simple_cmd;







#endif
