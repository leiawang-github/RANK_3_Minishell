一、struct sigaction 是什么

它是一个 信号行为控制结构体，定义在 <signal.h> 中，用来描述在接收到某个信号时，进程应当执行的动作。
sigaction 既是一个系统调用函数名，又是一个结构体类型名。两者同名，但它们是不同的东西，作用完全不同。

1. 系统定义大致如下（精简版）：
```c
struct sigaction {
    void (*sa_handler)(int);   // 信号处理函数指针（或SIG_IGN、SIG_DFL）
    sigset_t sa_mask;          // 在处理该信号时临时阻塞的其他信号
    int sa_flags;              // 控制行为的标志位（SA_RESTART等）
};
```
你得到一个变量 act，它内部保存三类信息：

收到信号后该干什么（sa_handler）

在处理该信号时应暂时屏蔽哪些其他信号（sa_mask）

额外行为标志（sa_flags）

2. 函数部分（系统调用）
```c
int sigaction(int signum, const struct sigaction *act,   struct sigaction *oldact);
//“请把信号 signum 的处理方式改成 act 描述的样子，并把原来的行为保存到 oldact。”
```

一个例子：
```c
#include <signal.h>
#include <stdio.h>

void handler(int signo)
{
    printf("Caught signal %d\n", signo);
}

int main(void)
{
    struct sigaction act;
    ft_bzero(&act, sizeof(struct sigaction));
    act.sa_handler = handler;  // 当接收到信号时，调用 handler 函数

    // 注册信号行为
    sigaction(SIGINT, &act, NULL);

    while (1)
        ; // 无限循环，等待 Ctrl-C

    return 0;
}
/*
act 是一个结构体变量；

sigaction() 是一个函数；

你把 act 交给 sigaction()，由内核据此更新 SIGINT 的行为。
*/
```
