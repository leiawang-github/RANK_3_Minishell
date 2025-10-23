```bash

               ┌────────────────────────────────────┐
               │              Shell 主进程           │
               └────────────────────────────────────┘
                          │
                          ▼
                   [解析命令行 input]
                          │
                          ▼
                 [only need to be executed in parent process? condition: single cmd + builtin ]
                          │
                   yes ───┘
                          ▼
               [保存父进程的标准fd状态]
               ┌──────────────────────────────┐
               │ save_parent_fds():           │
               │   dup(0) → saved_stdin_fd    │
               │   dup(1) → saved_stdout_fd   │
               │   dup(2) → saved_stderr_fd   │
               └──────────────────────────────┘
                          │
                          ▼
                  [应用重定向 apply_redirs()]
               ┌────────────────────────────────┐
               │ open("out.txt", O_WRONLY|O_CREAT|O_TRUNC) → fd4 │
               │ dup2(fd4, 1) → stdout                           │
               │ close(fd4)                                       │
               └────────────────────────────────┘
                          │
                          ▼
                   [执行内置命令 builtin]
               ┌────────────────────────────────┐
               │ exec_builtin("echo", ["hello"])│
               │ → write(1, "hello\n")          │
               │ （stdout 此时已指向 out.txt） │
               └────────────────────────────────┘
                          │
                          ▼
                 [恢复父进程的标准fd状态]
               ┌──────────────────────────────┐
               │ dup2(saved_stdin_fd, 0)      │
               │ dup2(saved_stdout_fd, 1)     │
               │ dup2(saved_stderr_fd, 2)     │
               │ close(saved_stdin_fd 等)     │
               └──────────────────────────────┘
                          │
                          ▼
                   [返回主循环，继续读取输入]
```
