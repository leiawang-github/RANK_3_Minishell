---------------------------- 1、what does parser do--------------

parser干什么？parser把tokens变成一条pipeline linked list，每个节点是一个简单命令，包含了简单命令（simple caommand）需要
具备的一个或多个元素；如果tokens里面有｜pipe，以pipe为界限截取节点。也就是说，如果token里面没有pipe，理论上只有一个节点；如果有一条pipe，有两个节点，如果有两条pipe，有三个节点。。。

-----------------------------2、基本实现--------------------------

整体流程应该满足：
a、split_by_pipe函数；将tokens（是一个linked list）分成多个nodes，返回的还是linked list
b、build_simple_cmd 函数：对上面的node内容进行扫描：如果是一个WORD类型（lexer里面定义的数字0）->推入cmd_argv;如果是重定向符号，也就是遇到了数字（2，3，4，5）->读取一个 WORD 作为 filename，挂到 redir 列表（并做互斥/覆盖规则检查）；任何语法错误（重定向后不是 WORD、段为空等）即时报错；更加具体执行：
  1）、if(WORD):若符合 NAME=VALUE 形式 且尚未见到命令名,说明是变量赋值 → 放进 assigns; 否则：若 argv 为空 → 作为命令名；否则 → 作为参数；我们需要先构造一个node的struct，里面有assignment（是一个链表），cmd_argv（类型是char**）；redir（类型是struct，里面包含重定向类型和后一个char * WORD）
  2）、if(2\3\4\5)也就是看到重定向符：期待下一个 token 必须是 WORD 作为文件名，然后挂到 redirs；
c、把所有简单命令段装进 t_pipeline{ cmds[], count } 或二叉 PIPE 节点链。？？？

- OVERALL WORKFLOW FOR PARSER
----------------------------
```rust
readline (user input line)
       │
       ▼
LEXER
  - lexical analysis: split input → into tokens (linked list)
       │
       ▼
PARSER
  - syntax analysis: scan tokens → build simple_cmd structs
                     (t_cmd = {assigns + cmd_argv + redirs + next})
       │
       ▼
   [ Global scan stage ]
       - split tokens by PIPE '|'
       - validate:
            * first token is PIPE? → syntax error
            * last token is PIPE?  → syntax error
            * PIPE next to PIPE?   → syntax error
       - each segment = one simple_cmd
   [ Simple command scan stage ]
       ├─ assigns (linked list)   (e.g. A=1, B=2)
       ├─ cmd_argv (char **)      (command name + arguments)
       └─ redirs (linked list)    (redirection records)
              │
              ▼
       Handle redirection tokens:
              scan: token → is redir token?
                   ├─ no  → treat as assign / argv
                   └─ yes → check next token is WORD?
                              ├─ no  → syntax error
                              └─ yes → build redir struct
                                        ├─ token == << (heredoc)
                                        │      ├─ redir->type = HEREDOC
                                        │      ├─ redir->delimiter = WORD
                                        │      ├─ redir->do_expand = (no quotes ? 1 :0)
                                        │
                                        └─ token in {<, >, >>}
                                               ├─ redir->type = IN / OUT / APPEND
                                               ├─ redir->target = WORD
                                               ├─ expand policy:
                                               │      - allow $VAR
                                               │      - allow ~
                                               │      - forbid IFS split
                                               │      - forbid glob
                                               └─ attach redir struct → cmd->redirs (push back)
       │
       ▼
EXECUTOR
  - for each simple_cmd in pipeline:
       1. apply redirs in order (dup2, open, heredoc temp file, etc.)
       2. setup pipes if needed
       3. finally call execve(cmd_argv[0], cmd_argv, envp)

```

----------------------------- 3、 关于redir部分的展开：在 parser 遇到重定向tokens时要做的事

a、读到重定向 token（<、>、>>、<<）后，下一个 token 必须是 WORD，如果不是WORD，返回错误。
b、生成一条“重定向记录”（挂到 cmd->redirs 尾部），这个记录包含：
- 类型（针对哪个方向/方式）：
	< → 从文件读到 stdin
	> → 截断写 stdout
	>> → 追加写 stdout
	<< → heredoc，后续会先把多行文本收集好，再接给 stdin
- 目标（右侧 WORD 的内容）：
	<、>、>> 的目标是路径字符串（文件名）
	<< 的目标是定界词（heredoc 结束标识）
c、heredoc 的额外标记（仅 <<）
	是否展开：定界词若原本带引号（在 lexer 已去掉引号，但会给你保留“带过引号”的标志），则 heredoc 内容不做变量展开；否则允许展开。
	记录定界词文本（去引号后的纯文本），并记一个布尔 do_expand（1/0）。
d、可选）给“重定向词”的扩展策略打标
	重定向右侧 WORD 在 expand 阶段：
	允许变量展开、波浪线 ~ 展开；
	不允许 IFS split（分裂为多个字段），也不做 glob（通配符）。

-------------------------------- in workflow ----------------------------

more on expander：
here_doc 后面 WORD token 需要被展开的情况包括：
	- 允许展开变量 ($VAR)
	例如：> $HOME/output.txt → $HOME 会展开为当前用户目录

	- 允许波浪线展开 (~)
	例如：> ~/file.txt → ~ 会展开为用户主目录

	- 禁止 IFS split（字段分裂）
	shell 默认会用 IFS（通常是空格、Tab、换行）分裂字符串成多个字段，但重定向路径不需要
	例如：> "my file.txt" → 不应该分裂成两个路径

	-禁止 glob（通配符匹配）
	*、?、[abc] 这些不应该在重定向路径中匹配文件名



More Notes：
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



