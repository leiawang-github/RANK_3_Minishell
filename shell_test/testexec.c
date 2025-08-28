
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// int main() {
//     pid_t pid = fork();
//     if (pid == 0) {
//         // 子进程执行命令
// 		printf("pid is: %d\n", pid);
//         execlp("ls", "ls", "-l", NULL);
//         perror("exec failed");
//         _exit(1);
//     } else {
//         // 父进程等待子进程
//         wait(NULL);
//         printf("Child finished.\n");
//     }
//     return 0;
// }

// int main()
// {
//     int fd = open("test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     printf("fd is: %d\n", fd);
// }

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// int main(void) {
//     // 1. 正常输出到屏幕
//     printf("step 1: hello, terminal!\n");

//     // 2. 备份 stdout (fd = 1)
//     int backup = dup(STDOUT_FILENO);

//     // 3. 打开文件 out.txt
//     int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd < 0) {
//         perror("open");
//         return 1;
//     }

//     // 4. 把 stdout 重定向到文件
//     dup2(fd, STDOUT_FILENO);
//     close(fd);

//     // 5. 现在 printf 不再输出到屏幕，而是写入 out.txt
//     printf("step 2: hello, file!\n");
//     fflush(stdout);

//     // 6. 恢复 stdout
//     dup2(backup, STDOUT_FILENO);
//     close(backup);

//     // 7. 又回到屏幕
//     printf("step 3: back to terminal!\n");

//     return 0;
// }

int main()
{
    int fd = open("test.md", O_RDONLY);
    if(fd < 0)
        perror();




}
