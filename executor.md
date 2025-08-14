INPUT: t_simple_cmd

- count how many nodes, namely, simple_cmds in the pipe linked list
- create count-1 pipes
- go through redirs linked list in t_simple_cmd



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
│ Step 1: 命令来源判定（先内建，后外部）                                   │
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
