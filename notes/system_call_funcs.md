1. dup

dup() 是 Unix/Linux 系统调用，作用是：复制一个已打开文件描述符（file descriptor，简称 fd），并返回一个新的文件描述符，它指向同一个打开的文件表项。

换句话说：
它不是重新打开文件，而是共享同一个文件偏移量和状态标志；
如果一个进程对其中一个 fd 执行读写操作，文件偏移量会在另一个 fd 上同步变化；
两者可以互相替换使用。

一、函数原型

```c
#include <unistd.h>

int dup(int oldfd);
/*
参数：oldfd 是一个已存在、有效的文件描述符（例如 0、1、2 分别对应 stdin、stdout、stderr）。

返回值：
成功时返回新的文件描述符（最小可用的 fd 号）；
失败时返回 -1，并设置 errno（例如 EBADF 表示旧 fd 无效）。*/

```
二、基本原理

每个进程维护一个 文件描述符表（file descriptor table），表中的每个条目指向一个 打开文件表项（open file table entry）。
dup() 就是在当前进程的文件描述符表中复制一个条目。

可以这样理解：
```bash
┌────────────────────────────┐
│        文件描述符表         │
│  fd=0 ──┐                  │
│  fd=1 ──┼──► open file entry──► 文件 inode
│  fd=2 ──┘                  │
└────────────────────────────┘
```
调用 dup(1) 后，会创建一个新的文件描述符，例如 3，它也指向同一个 open file entry：
```bash
3 ───► [same open file entry as 1]
```
这意味着：

文件偏移量（file offset）共享
文件状态标志（如 O_APPEND）共享
但两个文件描述符号独立（可以单独关闭）

三、典型用法
用来
```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); //open 函数会打开这个文件并返回一个fd
    printf("fd gets from output.txt is: %d", fd); // supposed to be 3; 因为012分别是这个file的stdin stdout stderr

    int saved_stdout = dup(1);   // 保存当前stdout副本；所谓的dup(1)的意思也就是dup(stdout) or dup(STDOUT_FILENO)

    dup2(fd, 1);                 // 重定向stdout到文件； 这里的意思是把 fd，也就是open函数的返回值变成新的stdout
    printf("This will go into output.txt\n");

    dup2(saved_stdout, 1);       // 恢复原来的stdout；这里的意思是把之前存过的旧的stdout重新接回stdout
    printf("This will show on screen\n");

    close(fd);
    close(saved_stdout);
    return 0;
}
```

四、画图表示
```lua
┌──────────────────────────────────────────────────────────┐
│                   PARENT PROCESS (SHELL)                 │
│                执行内建命令（不 fork）                    │
└──────────────────────────────────────────────────────────┘
                │
                ▼
       [1] 备份当前标准 fd
                │
                │   ┌──────────────────────────────┐
                ├──►│ parent_fds[0] = dup(0)      │  ← 备份 stdin
                │   │ parent_fds[1] = dup(1)      │  ← 备份 stdout
                │   │ parent_fds[2] = dup(2)      │  ← 备份 stderr
                │   └──────────────────────────────┘
                │
                ▼
       [2] 应用重定向 (apply_redirs)
                │
                │   ┌───────────────────────────────────────────┐
                ├──►│ open("infile", O_RDONLY) → fd_in          │
                │   │ open("outfile", O_WRONLY|O_CREAT|O_TRUNC) │
                │   │ dup2(fd_in, 0)                            │ → stdin 指向 infile
                │   │ dup2(fd_out, 1)                           │ → stdout 指向 outfile
                │   │ close(fd_in / fd_out)                     │
                │   └───────────────────────────────────────────┘
                │
                ▼
       [3] 执行 builtin
                │
                │   echo / cd / pwd / export ...
                ▼
       [4] 恢复父进程 fd
                │
                │   ┌──────────────────────────────┐
                ├──►│ dup2(parent_fds[0], 0);      │ → 恢复 stdin
                │   │ dup2(parent_fds[1], 1);      │ → 恢复 stdout
                │   │ dup2(parent_fds[2], 2);      │ → 恢复 stderr
                │   │ close(parent_fds[i])         │
                │   └──────────────────────────────┘
```

对比如果在外部命令执行阶段：
```lua
┌────────────────────────────────────────────────────────────┐
│                   PARENT PROCESS (SHELL)                   │
│             执行外部命令（需要 fork + execve）              │
└────────────────────────────────────────────────────────────┘
                │
                ▼
       [1] 创建子进程
                │
                ├──► pid = fork()
                │
                ├──► 若 pid > 0 → 父进程
                │       （等待子进程）
                │
                └──► 若 pid == 0 → 子进程
                        │
                        ▼
          ┌────────────────────────────────────────────┐
          │              CHILD PROCESS                 │
          └────────────────────────────────────────────┘
                        │
                [2] 应用重定向
                        │
                        │   ┌──────────────────────────────┐
                        ├──►│ if (redir->infile)           │
                        │   │     fd_in = open(infile);    │
                        │   │     dup2(fd_in, 0);          │ ← stdin → infile
                        │   │     close(fd_in);            │
                        │   │                              │
                        │   │ if (redir->outfile)          │
                        │   │     fd_out = open(outfile);  │
                        │   │     dup2(fd_out, 1);         │ ← stdout → outfile
                        │   │     close(fd_out);           │
                        │   └──────────────────────────────┘
                        │
                        ▼
                [3] 执行 execve()
                        │
                        │   ┌──────────────────────────────┐
                        ├──►│ execve(cmd->path, argv, env) │
                        │   └──────────────────────────────┘
                        │
                        ▼
                [4] 若 execve 成功 → 不再返回
                [5] 若失败 → perror + exit(errno)
```
