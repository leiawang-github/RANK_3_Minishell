#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("=== execve() 行为测试 ===\n");
    
    // 测试1：执行存在的命令
    printf("1. 测试成功的情况:\n");
    printf("   准备执行 execve\n");
    
    if (execve("/bin/ls", (char*[]){"ls", "-l", NULL}, NULL) == -1) {
        printf("   execve 失败: %s\n", strerror(errno));
        printf("   这行代码会执行！\n");
    } else {
        printf("   execve 成功，但这行代码永远不会执行！\n");
    }
    
    printf("   这行代码永远不会执行！\n");
    
    return 0;
}
