# Heredoc 设计原理与实现详解

# heredoc 数据流：父进程（shell） ↔ 子进程（heredoc） ↔ 命令执行阶段
```bash
                ┌──────────────────────────────────────────────────────┐
                │                    SHELL                        │
                │ (parent process)                                     │
                │ ──────────────────────────────────────────────────── │
                │                                                      │
                │  1️⃣ 调用 pipe(pfd)                                  │
                │      pfd[0] ── 读端 (父将来保存)                     │
                │      pfd[1] ── 写端 (子将来使用)                     │
                │                                                      │
                │  2️⃣ fork()                                           │
                │      │                                                │
                │      ├──► 子进程 (heredoc collector)                 │
                │      │                                                │
                │      └── 保留 pfd[0]，关闭 pfd[1]                    │
                │                                                      │
                │  3️⃣ 等待子进程                                       │
                │      waitpid(pid, &status, 0)                        │
                │                                                      │
                │  4️⃣ 若成功：                                         │
                │      redir->heredoc_fd = pfd[0];                     │
                │                                                      │
                └──────────────────────────────────────────────────────┘
                                     │
                                     │ heredoc_fd 保存了管道的读端
                                     ▼
┌────────────────────────────────────────────────────────────────────────┐
│                  执行阶段：命令子进程                                  │
│ ────────────────────────────────────────────────────────────────────── │
│  dup2(redir->heredoc_fd, STDIN_FILENO);                               │
│  close(redir->heredoc_fd);                                            │
│                                                                       │
│  ──► 命令执行时，STDIN 来自 heredoc 的管道读端                         │
└────────────────────────────────────────────────────────────────────────┘


        子进程 (heredoc collector)
        ┌──────────────────────────────────────────┐
        │ SIGINT = DFL   SIGQUIT = IGN             │
        │ ─────────────────────────────────────── │
        │ close(pfd[0]);                          │
        │ while (1):                              │
        │   line = readline("> ");                │
        │   if (line==NULL) break;                │
        │   if (strcmp(line, delimiter)==0) break;│
        │   write(pfd[1], line, len); write(pfd[1], "\n", 1); │
        │ close(pfd[1]); _exit(0);                │
        └──────────────────────────────────────────┘
                         │
                         │ (写入)
                         ▼
┌─────────────────────────────────────────────────────────────┐
│             内核匿名管道对象                                │
│ ─────────────────────────────────────────────────────────── │
│ [缓冲区: line1\n line2\n ...]                               │
│                                                             │
│ 被写端(pfd[1])写入的数据，供读端(pfd[0])在父进程中读取。  │
└─────────────────────────────────────────────────────────────┘


```
```bash
为什么需要在父进程屏蔽heredoc输入时可能收到的打断信号：

每个终端设备都有一个“前台进程组（foreground process group）”，
当你按下 Ctrl-C 时，终端驱动（tty driver）会向该组中所有进程发送 SIGINT。

默认情况下：

shell 启动时是前台进程组；

fork 出的子进程会继承父进程的进程组；

除非你调用 setpgid() 或 tcsetpgrp() 改变。

所以——
如果 heredoc 不单独设置进程组或不屏蔽信号，父进程也会收到 SIGINT，
主 shell 会被中断输入循环（例如 readline() 报错并返回 NULL）。

主流逻辑一般→ 逻辑是：

父进程暂时忽略 SIGINT；

子进程允许 SIGINT；

用户按 Ctrl-C → 只杀掉子进程；

父进程 waitpid() 得到返回，知道 heredoc 被中断；

shell 保持运行。


| 角色              | SIGINT (Ctrl-C)        | SIGQUIT (Ctrl-\)           |
| --------------- | ---------------------- | -------------------------- |
| **父进程 (shell)** | 临时忽略，防止被打断             | 临时忽略                       |
| **heredoc 子进程** | 设为默认动作：用户可 Ctrl-C 中断输入 | 忽略：避免 “Quit (core dumped)” |



```

























# 一、什么是Heredoc？

Heredoc（Here Document）是shell中的一种特殊重定向方式，允许在命令行中直接输入多行文本作为命令的输入。

### 基本语法
```bash
command << DELIMITER
line 1
line 2
line 3
DELIMITER
```

### 实际例子
```bash
# 创建多行文本文件
cat << EOF > file.txt
Hello World
This is a test
EOF

# 多行输入到命令
grep "pattern" << END
text line 1
text line 2
text line 3
END

# 在脚本中使用
mail -s "Subject" user@example.com << MAIL_END
Dear User,
This is the email body.
Regards
MAIL_END
```

---

## 二、为什么需要Heredoc？

### 1. **用户体验需求**

**问题场景**：
```bash
# 没有heredoc时，你需要这样做：
echo "line 1" > temp.txt
echo "line 2" >> temp.txt
echo "line 3" >> temp.txt
cat temp.txt
rm temp.txt

# 有了heredoc，可以直接：
cat << EOF
line 1
line 2
line 3
EOF
```

**优势**：
- 避免创建临时文件
- 代码更简洁易读
- 适合在脚本中嵌入配置或文档

### 2. **脚本编程需求**

在shell脚本中嵌入配置文件、SQL语句、邮件内容等：

```bash
#!/bin/bash
# 生成配置文件
cat << CONFIG > /etc/myapp.conf
[settings]
host=localhost
port=8080
debug=true
CONFIG

# 执行SQL语句
mysql -u root << SQL
CREATE DATABASE test;
USE test;
CREATE TABLE users (id INT, name VARCHAR(50));
SQL

# 发送格式化的邮件
mail -s "Daily Report" admin@company.com << EMAIL
Daily Statistics:
- Users: 1234
- Sales: $5678
- Status: All systems operational
EMAIL
```

### 3. **交互式输入的自动化**

某些程序需要交互式输入，heredoc可以自动化这个过程：

```bash
# 自动回答交互式程序的问题
program << ANSWERS
yes
option1
42
quit
ANSWERS

# 批量输入到交互式shell
ftp server.com << FTP
user username password
cd /upload
put file.txt
quit
FTP
```

---

## 三、Heredoc的技术挑战

### 挑战1：输入收集的时机问题

**问题**：Heredoc需要在命令执行前收集所有输入

```bash
ls | grep foo << EOF
pattern1
pattern2
EOF
```

执行流程应该是：
1. **先收集heredoc内容**（用户输入pattern1, pattern2）
2. 然后再执行管道（`ls | grep foo`）

如果在fork子进程后才收集heredoc，会导致：
- 多个子进程同时读取stdin（冲突）
- 用户不知道该给哪个命令输入
- readline提示符混乱

### 挑战2：多个Heredoc的处理

```bash
cmd1 << EOF1 | cmd2 << EOF2 | cmd3 << EOF3
data for cmd1
EOF1
data for cmd2
EOF2
data for cmd3
EOF3
```

必须**按顺序**收集所有heredoc，存储它们的内容，然后才能执行管道。

### 挑战3：信号处理

用户在输入heredoc时可能按Ctrl+C：

```bash
$ cat << EOF
heredoc> line 1
heredoc> ^C        # 用户按了Ctrl+C
$                  # 应该取消整个命令，返回提示符
```

需要正确处理：
- 子进程收集heredoc时被中断
- 整个命令应该被取消
- 返回状态码130
- 不执行后续命令

### 挑战4：管道和文件描述符管理

Heredoc使用管道传递内容：

```
[child收集输入] --pipe--> [父进程保存读端] --fork--> [子进程dup2到stdin]
```

需要仔细管理fd，避免：
- fd泄漏
- 管道阻塞
- 读写端关闭顺序错误

---

## 四、为什么需要Heredoc预处理？

### 设计决策：在execute_command()入口处预处理

```c
int execute_command(t_mini *pipeline, char **envp, t_env *env_list)
{
    // 第一步：预处理所有heredoc
    rc = preprocess_heredoc(pipeline, envp, &interrupted);
    if (rc != 0)
        return rc;  // 中断或错误，直接返回

    // 第二步：执行命令（单节点或管道）
    if (!pipeline->next)
        return exec_single_cmd(...);
    else
        return exec_pipeline(...);
}
```

### 为什么要预处理？

#### 原因1：避免stdin竞争

**错误做法**（在fork后收集）：
```
Parent: fork → wait
Child 1: readline("heredoc> ")  ← 从stdin读
Child 2: readline("heredoc> ")  ← 也从stdin读
Child 3: readline("heredoc> ")  ← 还从stdin读

结果：三个进程抢stdin，行为不可预测
```

**正确做法**（预处理）：
```
Parent: 串行收集所有heredoc → fork → wait
Child 1: 从redir->fd读（已准备好的内容）
Child 2: 从redir->fd读
Child 3: 从redir->fd读

结果：每个子进程读取预先准备好的内容
```

#### 原因2：用户体验

```bash
$ ls | cat << EOF | wc -l
heredoc> line 1    ← 用户知道在输入heredoc
heredoc> line 2
heredoc> EOF
      2            ← 然后命令执行
```

如果不预处理：
```bash
$ ls | cat << EOF | wc -l
(ls开始执行，cat还没准备好输入...)
(用户不知道该做什么)
(程序挂起或出错)
```

#### 原因3：中断处理

```bash
$ cmd1 << EOF1 | cmd2 << EOF2
heredoc> line 1
heredoc> ^C       ← Ctrl+C应该取消整个命令
$                 ← 不应该执行cmd1和cmd2
```

预处理时检测到中断：
```c
rc = preprocess_heredoc(pipeline, envp, &interrupted);
if (rc != 0)
    return rc;  // 130，不执行后续命令
```

#### 原因4：错误处理

如果heredoc收集失败（如磁盘满），应该在执行前就报错：

```bash
$ cat << EOF | grep foo
heredoc> (大量数据...)
heredoc> Error: No space left on device
$         ← 不执行cat和grep
```

---

## 五、Heredoc预处理的实现细节

### 架构设计

```
┌─────────────────────────────────────────────────────────────┐
│ preprocess_heredoc(pipeline)                                │
│                                                             │
│  遍历所有节点和重定向                                        │
│  ├─ node1                                                   │
│  │   ├─ redir1: << EOF1  → prepare_one_heredoc()          │
│  │   └─ redir2: > out                                      │
│  ├─ node2                                                   │
│  │   └─ redir1: << EOF2  → prepare_one_heredoc()          │
│  └─ node3                                                   │
│      └─ redir1: < in                                        │
└─────────────────────────────────────────────────────────────┘
```

### prepare_one_heredoc() 流程

```c
static int prepare_one_heredoc(t_redir *redir, char **envp, int *interrupted)
{
    int fds[2];
    pid_t child;

    // 1. 创建管道
    pipe(fds);  // fds[0]=read, fds[1]=write

    // 2. 父进程忽略信号（避免readline被中断）
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    // 3. Fork子进程收集输入
    child = fork();
    if (child == 0) {
        // 子进程：恢复信号，允许Ctrl+C中断
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        close(fds[0]);  // 关闭读端

        // 循环读取输入直到遇到delimiter
        while (1) {
            line = readline("heredoc> ");
            if (!line || strcmp(line, delimiter) == 0)
                break;

            // 可选：变量展开
            expanded = expand_variables(line);

            // 写入管道
            write(fds[1], expanded, strlen(expanded));
            write(fds[1], "\n", 1);

            free(line);
            free(expanded);
        }

        close(fds[1]);
        _exit(0);
    }

    // 4. 父进程：等待子进程
    close(fds[1]);  // 关闭写端
    waitpid(child, &status, 0);

    // 5. 检查子进程退出状态
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
        *interrupted = 1;
        close(fds[0]);
        return 130;  // Ctrl+C
    }

    // 6. 保存读端给后续使用
    redir->fd = fds[0];  // 保存管道读端

    // 7. 恢复信号处理
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);

    return 0;
}
```

### 信号处理的精妙设计

```
时间线：
─────────────────────────────────────────────────────────────
状态    │ 主循环  │ 预处理开始 │ Fork子进程 │ 等待完成 │ 恢复
─────────────────────────────────────────────────────────────
父进程  │ custom  │  SIG_IGN   │  SIG_IGN   │ SIG_IGN  │ custom
子进程  │   N/A   │    N/A     │  SIG_DFL   │  N/A     │  N/A
─────────────────────────────────────────────────────────────
原因    │ 新提示符│ 不中断等待 │ 允许Ctrl+C │ 同左     │ 回到主循环
```

**为什么父进程要SIG_IGN？**
- 如果父进程也响应Ctrl+C，waitpid会被中断
- 父进程会提前退出，子进程变成僵尸进程
- 应该让子进程处理Ctrl+C，然后通过exit status通知父进程

**为什么子进程要SIG_DFL？**
- 用户按Ctrl+C是想中断heredoc输入
- 子进程接收到SIGINT后会被终止
- 父进程通过`WIFSIGNALED(status)`检测到中断

### 资源清理的重要性

```c
int preprocess_heredoc(t_mini *pipeline, char **envp, int *interrupted)
{
    t_mini *node = pipeline;

    while (node) {
        t_redir *r = node->redirs;
        while (r) {
            if (r->redir_type == HEREDOC) {
                rc = prepare_one_heredoc(r, envp, interrupted);
                if (rc != 0) {
                    // 关键：清理所有已打开的heredoc fd
                    cleanup_heredoc_fds(pipeline);
                    return rc;
                }
            }
            r = r->next;
        }
        node = node->next;
    }
    return 0;
}
```

**为什么需要cleanup？**

场景：
```bash
cmd1 << EOF1 | cmd2 << EOF2 | cmd3 << EOF3
data1
EOF1
data2
^C          ← 用户在第二个heredoc时按Ctrl+C
```

此时：
- EOF1的fd已经打开（redir->fd = fds[0]）
- EOF2收集时被中断
- EOF3还没开始

如果不清理：
- EOF1的fd泄漏
- 下次执行命令时可能fd耗尽

---

## 六、Heredoc与其他重定向的交互

### 优先级规则

重定向按**出现顺序**应用，后面的覆盖前面的：

```bash
# heredoc被后面的<覆盖
cat << EOF < file.txt
ignored
EOF
# 实际读取file.txt，不是heredoc

# <被后面的heredoc覆盖
cat < file.txt << EOF
hello
EOF
# 实际读取heredoc，不是file.txt
```

### 实现

```c
int exec_redirs(t_redir *redirs)
{
    t_redir *r = redirs;

    while (r) {
        if (r->redir_type == IN)
            dup2(open(r->file), STDIN);      // 设置stdin
        else if (r->redir_type == HEREDOC)
            dup2(r->fd, STDIN);               // 覆盖stdin
        else if (r->redir_type == OUT)
            dup2(open(r->file), STDOUT);
        // ...
        r = r->next;
    }
}
```

每次`dup2`都会覆盖目标fd，所以后面的重定向生效。

---

## 七、变量展开

### 引号规则

```bash
# 无引号或双引号：展开变量
cat << EOF
Hello $USER        ← 展开为实际用户名
Your home: $HOME   ← 展开为实际路径
EOF

# 单引号或转义：不展开
cat << 'EOF'
Hello $USER        ← 保持字面值 $USER
Your home: $HOME   ← 保持字面值 $HOME
EOF

cat << \EOF
Same as 'EOF'      ← 不展开
\EOF
```

### 实现

Parser阶段设置标志：
```c
if (delimiter[0] == '\'' || delimiter[0] == '"' || delimiter[0] == '\\')
    redir->do_expand = 0;
else
    redir->do_expand = 1;
```

Heredoc收集时检查：
```c
if (redir->do_expand)
    expanded = expand_variables(line, envp);
else
    expanded = strdup(line);  // 保持原样
```

---

## 八、常见错误和陷阱

### 错误1：忘记关闭fd

```c
// ❌ 错误
pipe(fds);
fork();
// 忘记在父进程关闭fds[1]
// 子进程读取fds[0]会阻塞！
```

**原因**：管道的写端没有完全关闭，读端会一直等待。

### 错误2：在子进程中收集heredoc

```c
// ❌ 错误
pid = fork();
if (pid == 0) {
    // 子进程中readline
    line = readline("heredoc> ");  // 多个子进程竞争stdin！
}
```

### 错误3：信号处理不当

```c
// ❌ 错误
signal(SIGINT, SIG_IGN);
child = fork();
// 子进程继承SIG_IGN，用户按Ctrl+C无法中断！
```

应该在子进程中恢复SIG_DFL。

### 错误4：没有清理已打开的fd

```c
// ❌ 错误
prepare_heredoc1();  // 成功，打开了fds[0]
prepare_heredoc2();  // 失败
return -1;           // heredoc1的fd泄漏！
```

---

## 九、测试用例

### 基本功能
```bash
# 1. 简单heredoc
cat << EOF
hello
world
EOF

# 2. heredoc + 输出重定向
cat << EOF > output.txt
test
EOF

# 3. 管道中的heredoc
cat << EOF | grep hello
hello world
foo bar
EOF
```

### 边界情况
```bash
# 4. 空heredoc
cat << EOF
EOF

# 5. 只有空格的行
cat << EOF


EOF

# 6. delimiter是EOF的子串
cat << EOF
This line contains EOF but is not delimiter
EOF

# 7. 多个heredoc
cat << EOF1 << EOF2  # 第二个覆盖第一个
data1
EOF1
data2
EOF2
```

### 信号处理
```bash
# 8. Ctrl+C中断
cat << EOF
line 1
^C        # 应该返回提示符，不执行cat

# 9. 管道中heredoc被中断
ls | cat << EOF | wc -l
^C        # 整个管道不执行
```

### 变量展开
```bash
# 10. 展开变量
cat << EOF
USER=$USER
HOME=$HOME
EOF

# 11. 不展开变量
cat << 'EOF'
USER=$USER
HOME=$HOME
EOF
```

---

## 十、总结

### Heredoc存在的核心理由

1. **简化多行输入**：避免创建临时文件
2. **脚本可读性**：嵌入配置、文档、SQL等
3. **自动化交互**：替代手动输入
4. **标准shell特性**：bash、zsh等都支持

### 预处理的必要性

1. **避免stdin竞争**：多个子进程不能同时读stdin
2. **良好的用户体验**：按顺序提示用户输入
3. **正确的信号处理**：Ctrl+C应该取消整个命令
4. **错误处理**：失败时不执行后续命令

### 实现要点

1. **使用管道传递内容**：子进程收集 → 管道 → 父进程保存fd
2. **精心设计信号处理**：父进程SIG_IGN，子进程SIG_DFL
3. **资源管理**：失败时清理所有已打开的fd
4. **与其他重定向交互**：按顺序应用，后面覆盖前面

### 架构优势

```
readline → LEXER → PARSER → EXECUTOR
                              ↓
                    ┌─────────┴─────────┐
                    ↓                   ↓
            preprocess_heredoc    execute_command
                    ↓
            (所有heredoc准备好)
                    ↓
            exec_single / exec_pipeline
```

这种设计确保了：
- ✅ Heredoc在命令执行前完全准备好
- ✅ 不会有stdin竞争
- ✅ 信号处理正确
- ✅ 资源清理完善
- ✅ 符合bash行为

Heredoc是shell中一个看似简单，实则需要精心设计的特性。我们的实现遵循Unix哲学，正确处理了各种边界情况，提供了稳定可靠的heredoc功能。

