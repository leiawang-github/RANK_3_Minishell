## 对shell的介绍
1、shell接受三种读取信息的方式：
- 读取一个.sh文件：案例：在shell_test目录里运行 sh my_script.sh
- 通过 -c 参数 案例：在sh中运行：sh -c "echo hey you"
- 从 system() 或 popen() 调用  案例：运行目前文件夹中的test_shell.c文件

2、token识别
1️⃣ EOF 结束 token
规则：输入结束时，当前 token 自动截止。

示例：

```sh
echo hello
```
Token：echo、hello

EOF 到来后，hello 自动结束 token。

2️⃣ 运算符合并
规则：运算符字符如果可以与前一个组成合法运算符，合并在一起。

示例：

```sh
echo hi >> out.txt
```
Token 顺序：
1.echo
2.hi
3.>>（由两个 > 合并成一个运算符 token）
4.out.txt

3️⃣ 运算符截断
规则：如果下一个字符不能和当前运算符组成更长的运算符，当前运算符 token 结束。

示例：
```sh
echo hi > file
```
> 是单独运算符，因为下一个是空格。

4️⃣ 引号影响 token
规则：引号开始后，直到引号结束前，字符原样保留，形成同一个 token。

示例：
```sh
echo "hello world"
```
Token：
1.echo
2."hello world"（内部空格不分割 token）

5️⃣ 变量/命令替换起点
规则：遇到 $ 或 `，识别为展开/替换开始，但在 token 阶段不展开。

示例：
```sh
echo $HOME
echo `date`
```
Token：第一行

1.echo
2.$HOME（保持原样）

第二行：
1.echo
2.`date`（完整 token，保留反引号）

6️⃣ 新运算符开头
规则：遇到新运算符时，旧 token 截止，新 token 开始。

示例：
```sh
echo a|b
```
Token：
1.echo
2.a
3.|
4.b

7️⃣ 未引用换行符
规则：换行符结束当前 token。

示例：
```sh
echo hello
world
```
Token：
行1：echo hello
行2：world（上一行 token 截止在换行）

8️⃣ 未引用空白
规则：空格或 tab 结束前一个 token，本身丢弃。
示例：
```sh
ls   -l    /tmp
```
Token：ls、-l、/tmp
多余空格不会进入 token

9️⃣ 普通字符追加
规则：前一个 token 是 word，则当前普通字符追加到该 token。
示例：
```sh
hello
```
Token：hello
每个字符逐步追加，形成一个 word token。

🔟 注释
规则：# 到行尾（不含换行）被丢弃。

示例：
```sh
echo hi # this is comment
```
Token：echo、hi
# this is comment 被完全丢弃。

1️⃣1️⃣ 新单词起点
规则：若当前字符不属于其他情况，则新建一个 word token。

示例：
```sh
foo
```
第一个 f 开始一个新 word token。

1️⃣2️⃣ Token 截止后分类
规则：token 形成后，会被文法归类为：
word（命令或参数）
operator（|, &, ;, <, > 等）
io_here（<<）

示例：
```sh
echo hi > file
```
echo → word（命令）
hi → word（参数）
> → operator（重定向）
file → word（文件名）

3. simple command
命令文本从头到尾按以下步骤处理：

a. 保存变量赋值和重定向（不立即执行，留待后续步骤）
b. 对其他单词执行展开（Expansion）
c. 执行重定向
d. 执行变量赋值

例：
```sh
X=1 >out.txt echo hi
```
step1: X=1 记录为变量赋值;
step2: >out.txt 记录为重定向;
step3: echo hi 剩余部分先做展开;

!!⚠️对剩余部分展开包括：
- 波浪线展开（~）
- 参数展开（$VAR）
- 命令替换（`cmd` / $(cmd)）
- 算术展开（$((expr))）
- 通配符展开（*, ?）

展开后：
- 第一个字段 → 命令名
- 其他字段 → 命令参数


1. echo: 用来在终端输出一段文字或变量的值。
- echo -n 不换行输出
```bash
echo -n "hello"
echo " world
```
output： hello world

- echo -e :让转义符号生效
```bash
echo -e "hello\nworld"
```
output: hello
        world

- echo $ 表示要把变量名展开：输出值
```bash
VAR=123
echo $VAR
```
output：123


常见的可以展开的变量：  
- $HOME → 用户主目录路径
- $PATH → 系统搜索命令的路径列表
- $USER → 当前用户名
- $$ → 当前 shell 的进程号
- $? → 上一个命令的返回值 (0 表示成功)
- $# → 传入脚本的参数个数
- $@ → 脚本所有参数

dup/dup2

1. 背景知识

在 Linux/Unix 系统里，一切皆文件。

标准输入：stdin → 文件描述符 0

标准输出：stdout → 文件描述符 1

标准错误：stderr → 文件描述符 2

比如我们写：
```c
write(1, "hello\n", 6);
``` 
实际上就是往标准输出（FD=1）写。
而 dup/dup2 的作用就是：
👉 把一个已存在的文件描述符复制成另一个文件描述符。
复制后，两个 FD 指向同一个底层文件（共享文件偏移量、文件状态）。

