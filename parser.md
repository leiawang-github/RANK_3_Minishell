1、parser干什么？parser把tokens变成一条pipeline linked list，每个节点是一个简单命令，包含了简单命令（simple caommand）需要
具备的一个或多个元素；如果tokens里面有｜pipe，以pipe为界限截取节点。也就是说，如果token里面没有pipe，理论上只有一个节点；如果有一条pipe，有两个节点，如果有两条pipe，有三个节点。。。


readline ──> LEXER(tokens) ──> PARSER(simple_cmd 节点)   <-!parser就是处理这部分
                                   │
                                   ├─ assigns 链表（A=1…）
                                   ├─ argv 数组（命令名+参数）
                                   └─ redirs 链表（重定向记录）
                                                │
                                                └─ EXECUTOR 依次应用重定向 → execve()



整体流程应该满足：
a、split_by_pipe函数；将tokens（是一个linked list）分成多个节点，返回的还是linked list 里面的nodes
b、build_simple_cmd 函数：对上面的node内容进行扫描：如果是一个WORD类型（lexer里面定义的数字0）->推入cmd_argv;如果是重定向符号，也就是遇到了数字（2，3，4，5）->读取一个 WORD 作为 filename，挂到 redir 列表（并做互斥/覆盖规则检查）；任何语法错误（重定向后不是 WORD、段为空等）即时报错；更加具体执行：
  1）、if(WORD):若符合 NAME=VALUE 形式 且尚未见到命令名,说明是变量赋值 → 放进 assigns; 否则：若 argv 为空 → 作为命令名；否则 → 作为参数；我们需要先构造一个node的struct，里面有assignment，cmd_argv（类型是插入**）；redir（类型是？？）
  2）、if(2\3\4\5)也就是看到重定向符：期待下一个 token 必须是 WORD 作为文件名，然后挂到 redirs；
c、把所有简单命令段装进 t_pipeline{ cmds[], count } 或二叉 PIPE 节点链。？？？


parser构建pipe l四天的原理是构建一个BNF：  Backus–Naur Form： 一种用来形式化描述语言语法的符号系统，特别常见于编程语言、数据格式、通信协议的定义中

它是一种元语言（metalanguage）：用来描述另一种语言的规则。
你可以用它精确写出“某个语法结构是由哪些符号、以什么顺序组成”的定义。
主要由两种符号组成：

- 非终结符（non-terminal）：语法结构的名字，用尖括号包起来（<expr>、<stmt>）
- 终结符（terminal）：实际出现在语言中的字面符号（如关键字 if、字符 +、标识符）

规则： <非终结符> ::= 产生式
- ::= 读作 “定义为”
- 产生式可以由终结符、非终结符按一定顺序或组合构成
- 多种可能性用 | 表示（或）


一些例子：
*line = readline("echo -n > out.md | wc -l");
after lexer:

```ini
[WORD, "echo", next] -> [WORD, "-n", next] -> [REDIR_OUT, ">", next] -> [WORD,"out.md", next] -> [PIPE, "|", next] -> [WORD, "wc", next] -> [WORD, "-l", next];
```
after parser:
```ini
cmd0.argv   = ["echo","-n", NULL]
cmd0.redirs = [ (type = REDIR_OUT, filename="out.md") ]
cmd0.assigns= NULL
cmd0.next   = (未定)

cmd1.argv   = ["wc","-l", NULL]
cmd1.redirs = NULL
cmd1.assigns= NULL
cmd1.next   = NULL
```
构造cmd0->next = cmd1,即：
t_cmd* pipeline_head
    ┌──────────┐        next        ┌──────────┐
    │  cmd0    │ ─────────────────▶ │  cmd1    │ → NULL
    └──────────┘                    └──────────┘
      argv: echo -n                   argv: wc -l
      > out.md                        (no redir)







