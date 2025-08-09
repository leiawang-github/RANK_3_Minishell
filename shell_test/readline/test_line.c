#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


// int main(void) {
//     char *input;

//     while (1) {
//         input = readline("> ");  // 显示提示符并读取输入

//         if (input == NULL) {
//             printf("\nBye!\n");
//             break;  // Ctrl+D 退出
//         }

//         if (*input != '\0') {
//             add_history(input);  // 非空输入加入历史
//         }

//         if (strcmp(input, "exit") == 0) { //如果用户输入了exit就跳出
//             free(input);
//             break;
//         }

//         printf("You typed: %s\n", input);
//         free(input);  // readline() 使用 malloc 分配，需释放
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// Ctrl+L 清屏函数
int clear_screen(int count, int key) {
    (void)count;
    (void)key;
    printf("\033[H\033[J"); // ANSI 码：光标移到左上 + 清屏
    rl_on_new_line();       // 通知 readline 新行
    rl_redisplay();         // 重绘 readline 行
}

// Ctrl+R 模拟服务器消息（示例用）
int insert_message(int count, int key) {
    (void)count;
    (void)key;
    printf("\n[SERVER] 你有一条新消息！\n");
    rl_replace_line("", 0); // 清除输入行
    rl_on_new_line();
    rl_redisplay();
}

int main(void) {
    char *input;

    // 绑定快捷键
    rl_bind_key('\x0c', clear_screen);     // Ctrl+L 清屏
    rl_bind_key('\x12', insert_message);   // Ctrl+R 模拟服务器提示（不是真正历史搜索）

    // 主循环
    while (1) {
        input = readline("super_shell> ");

        // 用户按下 Ctrl+D
        if (input == NULL) {
            printf("\nBye! (EOF)\n");
            break;
        }

        // 如果输入为空，仅重绘提示符
        if (*input == '\0') {
            free(input);
            continue;
        }

        add_history(input); // 加入历史

        // 内置命令处理
        if (strcmp(input, "exit") == 0) {
            free(input);
            printf("Bye! (exit)\n");
            break;
        }
        else if (strcmp(input, "clear_history") == 0) {
            rl_clear_history();
            printf("✅ 历史记录已清空\n");
        }
        else if (strcmp(input, "hi") == 0) {
            printf("👋 Hello there, %s!\n", getenv("USER") ? getenv("USER") : "user");
        }
        else {
            printf("❓ Unknown command: %s\n", input);
        }

        free(input);
    }

    return 0;
}
