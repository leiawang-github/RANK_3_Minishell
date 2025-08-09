
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        // 子进程执行命令
		printf("pid is: %d\n", pid);
        execlp("ls", "ls", "-l", NULL);
        perror("exec failed");
        _exit(1);
    } else {
        // 父进程等待子进程
        wait(NULL);
        printf("Child finished.\n");
    }
    return 0;
}
