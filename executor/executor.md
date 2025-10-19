updated version:
step0: loop throught what parser offered us (should be a pipeline with a lot of simple cmds);
chech if there is a redir in each cmds, if there redir in cmd check if there is heredoc redir type in this redir. If also yes we prepocess heredoc first before we do anything.


```rust
Step0::prepare_all_heredocs(head)
┌──────────────────────────────────────────────────────────────┐
│                                                             │
│ • last_status = 0                                           │
│ • install_heredoc_signals()                                 │
│ • cmd = head                                                │
└──────────────┬───────────────────────────────────────────────┘
               ▼
while (cmd != NULL)
    │
    ├─ // 先判断是否存在 redir 链
    ├─ if (cmd->redirs == NULL) { cmd = cmd->next; continue; }
    │
    ├─ // 遍历该命令的每个 redir（只用 while）
    ├─ redir = cmd->redirs
    ├─ while (redir != NULL)
    │     │
    │     ├─ if (redir->redir_type != R_HDOC) {
    │     │       redir = redir->next;      // 不是 heredoc，跳过
    │     │       continue;
    │     │   }
    │     │
    │     ├─ // —— heredoc 规范化（分隔符与展开策略）——
    │     ├─ redir->delimiter   = strip_quotes(redir->target)
    │     ├─ redir->hdoc_expand = !has_any_quote(redir->target)
    │     ├─ redir->heredoc_fd  = -1
    │     │
    │     ├─ // 建立承载：匿名管道承接 heredoc 内容
    │     ├─ if (pipe(pfd) < 0) → goto FATAL
    │     │
    │     ├─ // 读入循环：直到遇到 delimiter / Ctrl-C / Ctrl-D
    │     ├─ while (1)
    │     │     ├─ line = readline("> ")
    │     │     ├─ if (line == NULL) {
    │     │     │     warn_hdoc_eof(redir->delimiter);   // Ctrl-D
    │     │     │     break;
    │     │     │   }
    │     │     ├─ if (exact_equal(line, redir->delimiter)) {
    │     │     │     free(line); break;
    │     │     │   }
    │     │     ├─ if (sigint_fired()) {
    │     │     │     free(line); close(pfd[0]); close(pfd[1]);
    │     │     │     last_status = 130; goto ABORT;
    │     │     │   }
    │     │     ├─ // 正常一行：按需 $ 展开 → 写入管道写端
    │     │     ├─ out = line
    │     │     ├─ if (redir->hdoc_expand) {
    │     │     │     out = expand_vars(line, env); free(line);
    │     │     │   }
    │     │     ├─ dprintf(pfd[1], "%s\n", out)
    │     │     └─ if (out != line) free(out)
    │     │
    │     ├─ // 收尾：关闭写端，保存读端供后续 dup2(STDIN)
    │     ├─ close(pfd[1])
    │     ├─ redir->heredoc_fd = pfd[0]
    │     ├─ mark_fd_for_cleanup_later(pfd[0])    // 父进程收尾统一关闭
    │     └─ redir = redir->next
    │
    └─ cmd = cmd->next
               │
               ▼
┌──────────────────────────────────────────────────────────────┐
│ 成功：restore_interactive_signals(); return 0                │
└──────────────────────────────────────────────────────────────┘

ABORT:  // SIGINT 中断 heredoc
    close_all_marked_heredoc_fds();
    restore_interactive_signals();
    return 130;

FATAL:  // pipe 等致命错误
    perror("pipe");
    close_all_marked_heredoc_fds();
    restore_interactive_signals();
    return 1;
```rust




readline → LEXER → PARSER
                    │
                    ▼
               head = t_cmd *        // parser 返回的 simple_cmd 链表头
                    │
                    ▼
         ┌──────────────────────────────┐
         │ one node only？(head->next)   │
         └───────────┬──────────────────┘
                     │
        YES (单命令)  │                 NO (pipeline，多命令)
                     │                                  │
                     ▼                                  ▼
      ┌───────────────────────────┐        ┌──────────────────────────────────┐
      │ Single Command Path       │        │ Pipeline Path                    │
      └──────────────┬────────────┘        └──────────────────┬──────────────┘
                     │                                        │
             is_builtin(argv[0]) ?                            │
           ┌──────────────┴────────────────────────           │
           │ YES: execute buildin in parent process│          │
           │ if (apply_redirs_in_parent()<0)       │          │
             {                                     │          │
                restore_fds(); return 1;           │          │
             }                                     │          │
           │  rc = run_builtin(cmd_argv);          │          │
           │  return rc                            │          │
           └───────────────────────────────────────┘          │
                     │                                        │
                     ▼                                        │
           NO: 外部程序（fork+execve）                          │
              pid = fork()                                    │
                if (pid == 0) {                               │
                    // Child:                                 │
                    //   1) 应用 redirs（在子进程）             │
                    //   2) execve(path, argv, envp)         │
                    //                                       │
                  } else {                                   │
                    // Parent: waitpid & collect status      │
                    return wait_and_collect()                │
                  }                                          │
                                                             ▼
                                             [预处理/按需建立管道：边走边建]
                                             prev_read = -1
                                             pids = []
                                             for (cmd = head; cmd; cmd = cmd->next):
                                                 // 若后继存在，建立“当前→下一个”的管道
                                                 if (cmd->next) pipe(cur_pipe) else cur_pipe={-1,-1}

                                                 pid = fork()
                                                 if (pid == 0) {              // ── 子进程 ──
                                                     // 1) 连接管道端
                                                     if (prev_read != -1) dup2(prev_read, STDIN)
                                                     if (cur_pipe[1] != -1) dup2(cur_pipe[1], STDOUT)
                                                     close_all_unused_fds(prev_read, cur_pipe)

                                                     // 2) 应用重定向（按出现顺序，覆盖前述 dup2）
                                                     if (apply_redirs_in_child(cmd->redirs) < 0) _exit(1)

                                                     // 3) 执行
                                                     if (is_builtin(cmd->argv[0]))
                                                         _exit(run_builtin(cmd->argv))
                                                     path = resolve_path(cmd->argv[0])   // 含 / 或 PATH 搜索
                                                     if (!path) _exit(127)
                                                     execve(path, cmd->argv, build_envp())
                                                     _exit((errno==ENOENT)?127:126)
                                                 }
                                                 // ── 父进程 ──
                                                 record_pid(pids, pid)
                                                 if (prev_read != -1) close(prev_read)     // 上一轮读端已交付
                                                 if (cur_pipe[1] != -1) close(cur_pipe[1]) // 立刻关写端，防阻塞
                                                 prev_read = (cur_pipe[0] != -1) ? cur_pipe[0] : -1

                                             // 循环结束
                                             if (prev_read != -1) close(prev_read)
                                             wait_all(pids) → 以“最后一个命令”的退出码更新 $?
                                             return last_status

```

Just an example：

- If the input is:
```bash
ls | grep foo | wc -l
```

- After lexer it will be a token linked list:
[WORD, "ls"] -> [PIPE,"|"] -> [WORD, "grep"] -> [WORD, "foo"] -> [PIPE, "|"] -> [WORD, "wc"] -> [WORD, "-l"]

- After Parser it will be a t_simple_cmd 链表:
```rust
┌───────────────────────────────┐
│ assign = NULL                 │  ← 环境变量赋值数组，NULL表示无
│ argv   = ["ls", NULL]         │  ← 命令 + 参数数组
│ redirs = NULL                 │  ← 重定向链表，NULL表示无
└─────────────┬─────────────────┘
              │ next
┌───────────────────────────────┐
│ assign = NULL                 │
│ argv   = ["grep", "foo", NULL]│
│ redirs = NULL                 │
└─────────────┬─────────────────┘
              │ next
┌───────────────────────────────┐
│ assign = NULL                 │
│ argv   = ["wc", "-l", NULL]   │
│ redirs = NULL                 │
└─────────────┬─────────────────┘
              │ next
             NULL

```

- After executor： first we know there will be three nodes, which means we need 2 pipes to connect them:
```rust
pipefds[0] = {fd[0], fd[1]}   // pipe 0：ls | grep foo
pipefds[1] = {fd[0], fd[1]}   // pipe 1：grep foo | wc -l
```

we need to arrange correct stdin/stdout for three t_simple_cmds, namely three processes:
```rust
Child Process 1 (ls)
stdin = default (terminal/file)
stdout = pipefds[0][1] (writes to grep's input)

Child Process 2 (grep foo)
stdin = pipefds[0][0] (reads from ls's output)
stdout = pipefds[1][1] (writes to wc's input)

Child Process 3 (wc -l)
stdin = pipefds[1][0] (reads from grep's output)
stdout = default (terminal)
```


```rust
altervative:
┌─────────────────────────────────────────────┐
│ Pipeline Path (n > 1)                       │
└──────────────────┬──────────────────────────┘
                   │
          for i = 0..n-1 over simple_cmd:
                ┌───────────────────────────────────────────────┐
                │ Step 1: 解析命令来源                           │
                └───────────────────┬───────────────────────────┘
                                    │
                      ┌─────────────┴───────────────┐
                      │ (含内建) 也在子进程中执行    │
                      │ （保持与其他命令一致）       │
                      └──────────────────────────────┘
                ┌───────────────────────────────────────────────┐
                │ Step 2: 外部程序路径确定（仅外部命令需要）     │
                └───────────────────┬───────────────────────────┘
                                    │
                    contains_slash(argv[0]) ?
                        YES → path = argv[0]
                        NO  → PATH 搜索（未命中→127）
                ┌───────────────────────────────────────────────┐
                │ Step 3: fork 子进程                           │
                └───────────────────┬───────────────────────────┘
                                    │
                               if (pid == 0) {
                                   // Child:
                                   //  a) 连接 pipeline 的 fd：
                                   //     i==0       → dup2(pipes[0].write, STDOUT)
                                   //     0<i<n-1    → dup2(pipes[i-1].read, STDIN);
                                   //                   dup2(pipes[i].write, STDOUT)
                                   //     i==n-1     → dup2(pipes[n-2].read, STDIN)
                                   //     并关闭所有无关 fd，避免阻塞
                                   //
                                   //  b) 应用 redirs（在子进程）
                                   //
                                   //  c) 执行：
                                   //     - 内建 → run_builtin(argv) 然后 _exit(return_code)
                                   //     - 外部 → execve(path, argv, envp)
                                   //             失败 → _exit(126/127)
                               } else {
                                   // Parent:
                                   //  - 记录 pid[i]
                                   //  - 关闭已无用的 pipe 端
                                   //  - 继续启动下一个命令
                               }
          // after loop:
          // 关闭父进程中仍未关闭的所有 pipe 端
          // waitpid(pid[0..n-1]) 并以“最后一个命令”的返回码作为 pipeline 返回值（与 bash 一致）



SingleCommandBuiltin(head):
    1) 语义校验
       - argv[0] 必须存在；若缺失 → 语法错误（不进入本流程）
       - 内建名属于 {echo, cd, pwd, export, unset, env, exit}
    2) 备份标准 IO
       - saved_stdin  = dup(STDIN)
       - saved_stdout = dup(STDOUT)
       - saved_stderr = dup(STDERR)
    3) 应用重定向（父进程）
       - 依出现顺序处理 redirs：
         <, >, >>, <<（heredoc）
       - 任一 open/dup2 失败 → 打印错误，恢复 fd，返回 1
    4) 执行内建
       - rc = run_builtin(argv)
       - 特例：exit
         * 若 exit 无法解析参数或溢出 → 按题意打印错误并返回相应码（不退出 shell）
         * 否则在外层据 rc 处理收尾退出（见下“退出策略”）
    5) 恢复标准 IO
       - dup2(saved_*, STD*)，并关闭副本
    6) 返回 rc（写入 $?）



在你的 executor 里：

外部命令：在 waitpid() 后，根据 WEXITSTATUS(status) 更新。

内建命令：run_builtin() 的返回值，直接写入。

redirection 失败：g_last_status = 1。

command not found：g_last_status = 127。

不可执行文件：g_last_status = 126。

信号终止：g_last_status = 128 + signum（与 bash 对齐）。
