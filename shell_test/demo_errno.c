#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
    printf("=== errno 和 perror() 演示 ===\n\n");
    
    // 演示1: 尝试进入不存在的目录
    printf("1. 尝试进入不存在的目录:\n");
    if (chdir("/nonexistent/directory") == -1) {
        printf("   errno = %d\n", errno);
        perror("   chdir");
    }
    printf("\n");
    
    // 演示2: 尝试进入根目录（通常需要权限）
    printf("2. 尝试进入根目录:\n");
    if (chdir("/root") == -1) {
        printf("   errno = %d\n", errno);
        perror("   chdir");
    }
    printf("\n");
    
    // 演示3: 尝试进入一个文件而不是目录
    printf("3. 尝试进入一个文件:\n");
    if (chdir("/etc/passwd") == -1) {
        printf("   errno = %d\n", errno);
        perror("   chdir");
    }
    printf("\n");
    
    return 0;
}
