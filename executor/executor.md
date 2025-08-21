INPUT: t_cmd *cmd；this is the returning value from parser

based on how many nodes in simple cmd list, we ...



给每个 simple_cmd 分配好 stdin/stdout 重定向：
第一个命令：stdout → pipes[0][1]
中间命令：stdin ← pipes[i-1][0], stdout → pipes[i][1]
最后一个命令：stdin ← pipes[n-2][0]
每个命令都要 fork 出一个子进程，dup2() 修改它们的标准输入输出，然后 execve() 执行。
父进程要关掉多余的 fd，并 wait 所有子进程。

┌──────────────────────────────────────────────────────────────────────┐
│ Step 0: 形态判定                                                      │
└───────────────┬──────────────────────────────────────────────────────┘
                │
      A) 仅赋值? (argv==NULL && redirs empty) ──► apply_assigns_to_shell() → return 0
                │
      B) 仅重定向? (argv==NULL && redirs !empty)
          backup_fds(); if (apply_redirs_in_parent()<0){restore_fds();return 1;}
          restore_fds(); return 0
                │
      C) 赋值+重定向(无命令名)?
          backup_fds(); if (apply_redirs_in_parent()<0){restore_fds();return 1;}
          apply_assigns_to_shell(); restore_fds(); return 0
                │
      D) 含命令名 (argv[0] 非空) ──────────────────────────────────────────────►

┌──────────────────────────────────────────────────────────────────────┐
│ Step 1: if more than one node（先内建，后外部）                                   │
└───────────────┬──────────────────────────────────────────────────────┘
                │
        is_builtin(argv[0]) ?
      ┌───────────────┴────────────────┐
      │ YES: BUILTIN                   │  NO: EXTERNAL
      ▼                                ▼
 backup_fds()
 if (apply_redirs_in_parent()<0){      ┌───────────────────────────────────────┐
   restore_fds(); return 1;            │ Step 2: 外部程序路径确定               │
 }                                     └───────────────┬───────────────────────┘
 env_snapshot s = env_save();                          │
 env_apply_assigns_temporarily(assigns);   contains_slash(argv[0]) ?
 int rc = run_builtin(argv);                      ┌───────────────┴─────────────┐
 env_restore(s);                                  │ YES: 给定路径               │
 restore_fds();                                   │   path = argv[0]            │
 return rc                                        │                             │
                                                  │ NO: PATH 搜索               │
                                                  │   PATH = getenv("PATH")     │
                                                  │   若为空/缺失 → print "cmd  │
                                                  │   not found" ; return 127   │
                                                  │   dirs = split(PATH, ':')   │
                                                  │   空项→当前目录 "."         │
                                                  │   逐目录拼接 dir+"/"+name   │
                                                  │   access(X_OK)==0 → 命中    │
                                                  │   全未命中 → 127            │
                                                  └───────────────┬─────────────┘
                                                                  │ path 就绪
                                                                  ▼
                                           ┌────────────────────────────────────┐
                                           │ Step 3: 外部程序执行               │
                                           └─────────────────┬──────────────────┘
                                                             │
                                                      pid = fork()
                                                      if (pid==0) {           // 子进程
                                                         if (apply_redirs_in_child()<0) _exit(1);
                                                         char **envp = build_env_with_assigns(assigns);
                                                         // 可选加分: stat 判断目录/ENOEXEC 兜底
                                                         execve(path, argv, envp);
                                                         // 只有失败才会到达这里
                                                         // errno==ENOEXEC 可尝试 execl("/bin/sh","sh",path,NULL)
                                                         _exit( (errno==ENOENT)?127:126 );
                                                      }
                                                      // 父进程
                                                      return wait_and_collect(pid)

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


```bash
┌──────────────────────────────────────────────────────────────────────┐
│                    single cmd（no pipe：head->next == NULL）          │
├──────────────────────────────────────────────────────────────────────┤
│  process t_cmd (head)                                                           │
│     │                                                                           │
│     ├─ get cmd->type                                                            │
│     │   │                                                                       │
│     │   ├─ CMD_EMPTY ............→ return 0                                     │
│     │   ├─ CMD_ASSIGN_ONLY ......→ parent process：apply_assigns_to_shell        │
│     │   ├─ CMD_REDIR_ONLY .......→ parent process：backup→apply_redirs→restore   │
│     │   ├─ CMD_ASSIGN_REDIR .....→ parent process：backup→redirs→assigns→restore │
│     │   └─ CMD_WITH_ARGV                                                         │
│     │        │                                                                   │
│     │        ├─ is_builtin? → parent process：backup→redirs→(assigns)→run_builtin │
│     │        │                     → restore → 返回内建退出码                       │
│     │        └─ extra funs         → fork 子进程：redirs/assigns→execve               │
│     │                              父进程 wait → interpret_status                   │
└──────────────────────────────────────────────────────────────────────┘

```
```bash
┌──────────────────────────────────────────────────────────────────────┐
│                    多节点（有管道：head->next != NULL）               │
├──────────────────────────────────────────────────────────────────────┤
│  遍历链表 (for cur=head; cur; cur=cur->next)                         │
│     │                                                                │
│     ├─ 若不是最后一个节点 → pipe()                                    │
│     ├─ fork()                                                        │
│     │   │                                                            │
│     │   ├─ 子进程：                                                   │
│     │   │   ├─ 若有上游 prev_r → dup2(prev_r, STDIN)                 │
│     │   │   ├─ 若有下游 pipefd[1] → dup2(pipefd[1], STDOUT)          │
│     │   │   ├─ 关闭多余 pipe 端                                      │
│     │   │   ├─ apply_redirs_in_child(cur->redirs)                    │
│     │   │   └─ 根据 cmd->type 执行：                                 │
│     │   │        • CMD_EMPTY ............→ _exit(0)                  │
│     │   │        • CMD_ASSIGN_ONLY ......→ apply_assigns_to_child_env→_exit(0)
│     │   │        • CMD_REDIR_ONLY .......→ _exit(0)                  │
│     │   │        • CMD_ASSIGN_REDIR .....→ apply_assigns_to_child_env→_exit(0)
│     │   │        • CMD_WITH_ARGV                                   │
│     │   │             · 内建 → _exit(run_builtin)                   │
│     │   │             · 外部 → execve_with_path / 失败→_exit(127)   │
│     │   │                                                            │
│     │   └─ 父进程：                                                  │
│     │       ├─ 关闭上轮读端 prev_r                                   │
│     │       ├─ 关闭本轮写端 pipefd[1]                                │
│     │       └─ 把 pipefd[0] 保存为下一轮的 prev_r                    │
│     │                                                                │
│  循环结束：关闭残余 prev_r → wait 所有子进程 → 返回“最后一个”的退出码  │
└──────────────────────────────────────────────────────────────────────┘
```
