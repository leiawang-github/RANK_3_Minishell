#include "minishell.h"

// lexer.c  —— 最小演示版：把一行切成 tokens（WORD/PIPE/重定向），支持引号成对
#include <stdio.h>    // printf
#include <stdlib.h>   // malloc, free
#include <string.h>   // strlen, strncpy
#include <ctype.h>    // isspace

//============================= 小工具函数 ===============================

// 复制 input[pos..pos+len-1] 为新字符串                         // [L14]
static char *substr(const char *s, int pos, int len) {          // [L15]
    char *out = (char*)malloc(len + 1);                         // [L16]
    if (!out) return NULL;                                      // [L17]
    memcpy(out, s + pos, len);                                  // [L18]
    out[len] = '\0';                                            // [L19]
    return out;                                                 // [L20]
}

// 判断是否是 | < > 这些“特殊分隔符”（用于终止一个 WORD）          // [L21]
static int is_special(char c) {                                 // [L22]
    return (c == '|' || c == '<' || c == '>');                  // [L23]
}

// 把一个 token 接到链表尾（head/tail 指针双引用传入）             // [L24]
static void add_token(t_token **head, t_token **tail,           // [L25]
                      t_toktype type, const char *lexeme) {     // [L26]
    t_token *node = (t_token*)malloc(sizeof(t_token));          // [L27]
    if (!node) return;                                          // [L28]
    node->type = type;                                          // [L29]
    node->lexeme = strdup(lexeme);                              // [L30]
    node->next = NULL;                                          // [L31]
    if (!*head) *head = node;                                   // [L32]
    else (*tail)->next = node;                                  // [L33]
    *tail = node;                                               // [L34]
}

// 打印并释放 tokens（调试/演示用）                                // [L35]
static void print_tokens(t_token *toks) {                       // [L36]
    int i = 1;                                                  // [L37]
    while (toks) {                                              // [L38]
        printf("%d. { type = %d, lexeme = \"%s\" }\n",          // [L39]
               i, toks->type, toks->lexeme);                    // [L40]
        toks = toks->next;                                      // [L41]
        i++;                                                    // [L42]
    }                                                           // [L43]
}
static void free_tokens(t_token *toks) {                        // [L44]
    while (toks) {                                              // [L45]
        t_token *n = toks->next;                                // [L46]
        free(toks->lexeme);                                     // [L47]
        free(toks);                                             // [L48]
        toks = n;                                               // [L49]
    }                                                           // [L50]
}


//============================= 核心：lexer ==============================

t_token *lexer(const char *input) {                             // [L51]
    t_token *head = NULL, *tail = NULL;                         // [L52]
    int i = 0;                                                  // [L53]

    while (input[i]) {                                          // [L54]
        // 1) 跳过引号外的空白 —— 这时就用到了 { i++; continue; }   // [L55]
        if (isspace((unsigned char)input[i])) {                 // [L56]
            i++;                                                // [L57]
            continue;       // 继续下一轮循环                      // [L58]
        }                                                       // [L59]

        // 2) 处理运算符：|  <<  >>  <  >                           // [L60]
        if (input[i] == '|') {                                  // [L61]
            add_token(&head, &tail, TOK_PIPE, "|");             // [L62]
            i++;                                                // [L63]
            continue;                                           // [L64]
        }                                                       // [L65]
        if (input[i] == '<') {                                  // [L66]
            if (input[i+1] == '<') {                            // [L67]
                add_token(&head, &tail, TOK_HEREDOC, "<<");     // [L68]
                i += 2;                                         // [L69]
            } else {                                            // [L70]
                add_token(&head, &tail, TOK_REDIR_IN, "<");     // [L71]
                i++;                                            // [L72]
            }                                                   // [L73]
            continue;                                           // [L74]
        }                                                       // [L75]
        if (input[i] == '>') {                                  // [L76]
            if (input[i+1] == '>') {                            // [L77]
                add_token(&head, &tail, TOK_REDIR_APP, ">>");   // [L78]
                i += 2;                                         // [L79]
            } else {                                            // [L80]
                add_token(&head, &tail, TOK_REDIR_OUT, ">");    // [L81]
                i++;                                            // [L82]
            }                                                   // [L83]
            continue;                                           // [L84]
        }                                                       // [L85]

        // 3) 普通 WORD（支持成对引号把内部空格包起来）              // [L86]
        int start = i;                                          // [L87]
        int in_squote = 0, in_dquote = 0;                       // [L88]

        while (input[i]) {                                      // [L89]
            char c = input[i];                                  // [L90]
            if (!in_dquote && c == '\'') {                      // [L91]
                in_squote = !in_squote;                         // [L92]
                i++;                                            // [L93]
                continue;                                       // [L94]
            }                                                   // [L95]
            if (!in_squote && c == '\"') {                      // [L96]
                in_dquote = !in_dquote;                         // [L97]
                i++;                                            // [L98]
                continue;                                       // [L99]
            }                                                   // [L100]

            // 引号外：遇到空白或 |<> 就结束这个 WORD                 // [L101]
            if (!in_squote && !in_dquote &&                     // [L102]
                (isspace((unsigned char)c) || is_special(c))) { // [L103]
                break;                                          // [L104]
            }                                                   // [L105]

            i++;                                                // [L106]
        }                                                       // [L107]

        // 取出 [start, i) 作为一个 WORD token                      // [L108]
        char *lex = substr(input, start, i - start);            // [L109]
        if (!lex) { free_tokens(head); return NULL; }           // [L110]
        add_token(&head, &tail, TOK_WORD, lex);                 // [L111]
        free(lex);                                              // [L112]
        // 注意：这里不处理引号剥离/变量展开，这些留给 expander       // [L113]
    }                                                           // [L114]

    return head;                                                // [L115]
}


//============================= 一个小 main 用来演示 =====================

int main(void) {                                                // [L116]
    const char *line = "echo \"a b\" c | wc -l";                // [L117]
    t_token *toks = lexer(line);                                // [L118]
    if (!toks) return 1;                                        // [L119]
    print_tokens(toks);                                         // [L120]
    free_tokens(toks);                                          // [L121]
    return 0;                                                   // [L122]
}
