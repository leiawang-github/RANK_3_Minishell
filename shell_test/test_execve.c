#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("测试 execve() 的行为\n");
    
    // 测试1：执行存在的命令
    printf("1. 执行存在的命令 (ls):\n");
    if (execve("/bin/ls", (char*[]){"ls", "-l", NULL}, NULL) == -1) {
        printf("   execve 失败: %s\n", strerror(errno));
    }
    
    // 这行代码永远不会执行，因为 execve 成功时会替换进程
    printf("   这行代码不会执行！\n");
    
    return 0;
}
