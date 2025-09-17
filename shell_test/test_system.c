#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>

// int main() {
//     system("echo 'Hello from system()!'");
//     return 0;
// }


// int main()
// {
//     // fprintf(stderr, "oops: %s\n", "no file");
//     // printf("oops: %s\n", "no file");

//     if (chdir("/User/leia/desktop/out.md/hey") == -1) 
//     { //如果更改目录失败
//     perror("cd");  // 输出: cd: No such file or directory
//     }

    
// }

// #include <unistd.h>
// #include <stdio.h>
// #include <errno.h>

// int main(void) {
//     if (chdir("/tmp//tmp/myfile.txt/somewhere") != 0) {
//         perror("cd");
//         return 1;
//     }
//     return 0;
// }

// #include<stdio.h>
// #include<stdlib.h> // for getenv setenv
// // extern char** environ;
// int main(int argc, char* argv[]){
//     printf("Start...\n");
//     fflush(stdout);
//     char* lan = getenv();
//     printf("$PATH: %s\n", lan);
//     fflush(stdout);
//     char* path = getenv("PATH");
//     printf("$PATH: %s\n", path);
//     fflush(stdout);
//     setenv("HELLO", "WORLD", 1);
//     char* hello = getenv("HELLO");
//     printf("$HELLO: %s\n", hello);
//     fflush(stdout);
//     printf("End\n");
//     fflush(stdout);
//     return 0;
// }

// static void safe_write(int fd, const char *s) {
//     ssize_t n = write(fd, s, strlen(s));   // 系统调用：write()
//     if (n < 0) {
//         perror("write");
//         exit(1);
//     }
// }

// int main(void) {
//     // 1) 读取环境变量（系统/进程环境中的键值对，用户态可直接访问）
//     const char *path = getenv("PATH");   // 非系统调用,单纯读取环境变量
//     printf("[ENV] PATH = %s\n", path);
//     printf("\n");

//     // 2) 直接用系统调用 write() 输出（绕过 stdio 缓冲，直接写到文件描述符 1）
//     safe_write(1, "[SYS] Hello via write()\n");
//     printf("\n");


//     // 3) 使用系统调用 fork() 创建子进程
//     pid_t pid = fork();                  // 系统调用：fork()
//     if (pid < 0) {                       // 出错
//         perror("fork");
//         return 1;
//     }

//     char buf[128];
//     if (pid == 0) {
//         // 子进程
//         int len = snprintf(buf, sizeof(buf),
//                            "[CHILD] pid=%d, PATH begins with: %.20s\n",
//                            (int)getpid(), path ? path : "(null)");
//         if (len < 0) return 1;
//         safe_write(1, buf);              // 系统调用：write()
//         return 0;                        // 子进程直接退出
//     } else {
//         // 父进程
//         int status = 0;
//         if (waitpid(pid, &status, 0) < 0) {   // 系统调用：waitpid()
//             perror("waitpid");
//             return 1;
//         }
//         if (WIFEXITED(status)) {
//             int code = WEXITSTATUS(status);
//             int len = snprintf(buf, sizeof(buf),
//                                "[PARENT] child %d exited with %d\n",
//                                (int)pid, code);
//             if (len < 0) return 1;
//             safe_write(1, buf);               // 系统调用：write()
//         }
//     }
//     return 0;
// }

#include <stdio.h>
#include <unistd.h>

// int main(void) {
//     //printf("hello");   // 使用 stdio，行缓冲
//     // 没有换行，可能看不到输出
//     //sleep(2);
//     write(1, "world", 5); // 系统调用，立刻写出
//     // sleep(2);
//     // return 0;
// }



// int main(void) {
//     printf("X");               // stdio 缓冲 (没有换行符)
//     write(1, "Y", 1);          // 系统调用，直接写内核
//     sleep(2);                  // 等两秒
//     //printf("\n");              // 触发 flush
//     return 0;
// }

#include <stdio.h>
#include <unistd.h>

// int main(void) {
//     printf("X");       // 没有换行，先写到 stdio 缓冲
//     write(1, "Y", 1);  // 直接系统调用，立刻显示

//     sleep(2);

//     // 换成 exit(0) 和 _exit(0) 各自试试
//     exit(0);       // 会 flush 缓冲，X 最终会出现
//     //_exit(0);          // 不会 flush，X 会丢掉
// }

#include <stdio.h>

int main(void) {
    printf("Hello, world!\n");
    return 0;
}


