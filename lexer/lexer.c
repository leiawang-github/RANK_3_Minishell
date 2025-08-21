#include "minishell.h"



static void add_token(t_token **head, t_token **tail,           // [L25]
                      t_token_type type, const char *lexeme) {     // [L26]
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

t_token *lexer(const char *input)
{
    t_token *head = NULL, *tail = NULL;
    int i = 0;

    while (input[i])
    {
        if (isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }
        if (input[i] == '|')
        {
            add_token(&head, &tail, T_PIPE, "|");
            i++;
            continue;
        }
        if (input[i] == '<') {
            if (input[i+1] == '<')
            {
                add_token(&head, &tail, T_HEREDOC, "<<");
                i += 2;
            } else
            {
                add_token(&head, &tail, T_REDIR_IN, "<");
                i++;
            }
            continue;
        }
        if (input[i] == '>') {
            if (input[i+1] == '>')
            {
                add_token(&head, &tail, T_REDIR_APP, ">>");
                i += 2;
            } else {
                add_token(&head, &tail, T_REDIR_OUT, ">");
                i++;
            }
            continue;
        }
        int in_squote = 0, in_dquote = 0;
        t_sbuf sb;
        if (!sbuf_init(&sb, 32)) { free_tokens(head); return NULL; }

        while (input[i]) {
            char c = input[i];
            if (!in_dquote && c == '\'') { in_squote = !in_squote; i++; continue; }
            if (!in_squote && c == '\"') { in_dquote = !in_dquote; i++; continue; }

            if (!in_squote && !in_dquote &&
                (isspace((unsigned char)c) || is_special(c))) {
                break;
            }


            if (!sbuf_putc(&sb, c)) { free_tokens(head); return NULL; }
            i++;
        }

        if (in_squote || in_dquote) {
            fprintf(stderr, "minishell: syntax error: unclosed quote\n");
            // g_status = 2; // 如果你有全局状态
            free(sb.buf);
            free_tokens(head);
            return NULL;
        }

        // 取出无引号的词并发出 WORD token
        char *lex = sbuf_take(&sb);
        if (!lex) { free_tokens(head); return NULL; }
        add_token(&head, &tail, T_WORD, lex);
        free(lex);
    }

    return head;
}


int main(void)
{
    const char *line = "echo -n > out.md | wc -l";
    t_token *toks = lexer(line);
    if (!toks) return 1;
    print_tokens(toks);
    free_tokens(toks);
    return 0;
}
