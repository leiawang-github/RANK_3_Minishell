1. 你的 minishell 不需要解释或支持：未闭合的引号（例如：echo "hello → 不用处理补全，直接报错或忽略）;特殊字符 \（反斜杠） 和 ;（分号）

2. 当用户输入 '单引号' 包裹的字符串时，内部所有字符都原样保留，shell 不应解释其中的 $、|、> 等符号。

3. 双引号内部也是防止大多数元字符被解释，但 保留 $ 的变量替换功能。

```bash
echo "Hello $USER"  #输出：Hello Leia， 这里可以看出即使在双引号里面，$符号被展开了
echo "Hello $USER > out.txt"  #输出： Hello leia > out.txt ,这里在双括号里，理论上所有的都应该被原样输出，但是$符号被展开了，但是重定向符号没有被展开：表现就是没有创建新文件
```

4. 应用重定向符号
```bash
echo hello > out.txt
```
当我们按下回车键后，执行的是：

a. Shell 读取整行命令。

b. 词法分析（Lexing）
→ Shell 把整行拆成 token：
["echo", "hello", ">", "out.txt"]

c. 语法分析（Parsing）
→ Shell 识别出：echo 是命令 / hello 是参数 / > 是重定向操作符 / out.txt 是重定向目标文件

d. 处理重定向（Redirection Setup） ✅
此步骤在执行命令(execve)之前完成

Shell会：
- 打开（或创建）out.txt 文件
- 获取它的文件描述符（假设是 fd=3）
- 把标准输出（fd=1）复制到 fd=3
- 关闭原来的 fd=1
- 让 fd=1 指向 out.txt

e. 执行命令本身（Execution）
- Shell 调用 fork() 创建子进程
- 子进程中执行 execve("/bin/echo", ["echo","hello"], envp)
- 程序开始运行，此时它的 stdout（1）已经指向文件

f. 写入数据
- echo 向 stdout 写入 "hello\n"
- 数据被写入 out.txt

g. Shell 收回控制权
命令执行完毕后，Shell 恢复自己的 I/O 状态


