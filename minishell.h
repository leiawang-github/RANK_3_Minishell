/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leia <leia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 21:11:44 by leiwang           #+#    #+#             */
/*   Updated: 2025/08/12 23:30:25 by leia             ###   ########.fr       */
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
    t_redir_type redir_type;
    union {
        struct {          // < , > , >>
            char   *path; // 文件名（变量展开后；不做 IFS split / 不做 glob）
        } file;
        struct {          // <<EOF
            char   *delim;     // 定界词（已去引号）
            int     do_expand; // 1=heredoc 内容允许 $ 展开；0=完全字面（由定界词是否带引号决定）
            char   *tmp_path;  // 预处理后存放内容的临时文件（heredoc 阶段生成）
        } hdoc;
    } u;
    struct s_redir *next;
} t_redir;

//cmd_node_format
typedef struct s_cmd {
    t_assign  *assigns; //是一个链表，因为可以有多个赋值
    char     **cmd_argv;           // ["echo","-n",NULL]
    t_redir_type   *redirs; //是一个链表，因为可能有多个重定向符号，每个重定向符号类型+word构成了链表节点
    int        assigns_only;   // 没有 argv，仅赋值：A=1 B=2   → 影响环境处理
    struct s_cmd *next;        // 管道里的下一个简单命令
} t_cmd;








#endif
