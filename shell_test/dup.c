#include <stdio.h>
#include <stdlib.h>

// int main(void) {
//     FILE *fp = fopen("test.txt", "r");  // 打开文件，模式是只读
//     if (!fp) {
//         perror("fopen");
//         return 1;
//     }

//     char buffer[128];
//     while (fgets(buffer, sizeof(buffer), fp))
//     {
//         printf("fopen read: %s", buffer);
//     }
//     fclose(fp);  // 关闭文件
//     return 0;
// }

#include <fcntl.h>   // open
#include <unistd.h>  // read, close
#include <stdio.h>
#include <stdlib.h>

// int main(void) {
//     // int fd = open("test.txt", O_RDONLY);  // 打开文件，模式是只读
//     // if (fd == -1) {
//     //     perror("open");
//     //     return 1;
//     // }

//     // char buffer[128];
//     // ssize_t bytes;
//     // while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
//     //     buffer[bytes] = '\0';  // 手动加结束符
//     //     printf("open read: %s", buffer);
//     // }

//     // if (bytes == -1) {
//     //     perror("read");
//     // }

//     // close(fd);  // 关闭文件
//     // return 0;

//     int fds[100];
//     pipe(fds);
//     printf("%d %d\n", fds[0], fds[1]);

// }


#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int saved_stdout = dup(1);   // 保存当前stdout副本

    dup2(fd, 1);                 // 重定向stdout到文件
    printf("This will go into output.txt\n");

    dup2(saved_stdout, 1);       // 恢复原来的stdout
    printf("This will show on screen\n");

    close(fd);
    close(saved_stdout);
    return 0;
}

