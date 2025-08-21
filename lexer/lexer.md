任务明晰：

1、 lexer的任务是什么？就是将用户输入切成特定类型的tokens，以便于在parser中使用。用户输入分成六类：
- WORD 除了一下几种标识的剩下的都按照word处理
- PIPE (|)
- REDIR_IN (<)
- REDIR_OUT (>)
- REDIR_APP (>>)
- HEREDOC (<<)
只要是符合lexer参数要求都能被切割成token，不可以的情况就是如输入里面有不完整的单引号或者双引号，这时候lexer返回错误信息。

2、lexer是如何切割input的？普通模式下是按照空格符号切割的，但如果在引号中，不管是单引号还是双引号，不切割，所以：
实现lexer其实是实现“状态机”---finite state machine， fsm：随着“当前状态”不同，对同一个输入字符采取不同的处理分支
why？
- 普通模式下遇到空格 → token 结束；
- 引号模式下遇到空格 → 空格是 token 内容一部分。
- < → 可能是 LESS 或 DLESS（看下一个字符是不是 <）。
- > → 可能是 GREAT 或 DGREAT。
- 在 here-doc 的定界词后，内容不展开变量（这也要依赖前一个 token 的状态）。
- 单引号模式下的 $ → 不展开；双引号模式下的 $ → 允许展开（这是 expand 阶段用到的，但引号模式信息必须在 lexer 保存下来）。


关于引号问题的处理：
- 保留 token 的“内容”，去掉引号本身
- 引号的作用是阻止分词和控制变量展开（单引号禁止展开，双引号允许展开）。
所以 lexer 需要做的不是直接删掉引号，而是：
-- 识别一对完整引号内的内容
-- 按规则决定是否保留 $ 等特殊字符
-- 把结果拼进一个完整的 WORD token

```bash
输入: echo "hello world"
lexer 输出: [WORD:"echo"] [WORD:"hello world"]
（引号去掉了，但空格被保留在内容中）
```
```bash
输入: echo 'a $b'
lexer 输出: [WORD:"echo"] [WORD:"a $b"]
（引号去掉了，并且 $b 保留原样，不展开）
```


for example1:
if *line = readline("echo -n > print.md") was given，after lexer, this should be provided:
1. { type = 0, lexeme = "echo" }
2. { type = 0, lexeme = "-n" }
3. { type = 3, lexeme = ">" }
4. { type = 0, lexeme = "print.md" }

example2:
*line = readline("'bin/local' ./sccript"); after lexer:
1. { type = 0, lexeme = "'bin/local'" }
2. { type = 0, lexeme = "./sccript" }
note that we don't need to handle single quotes just keep it here


example3:
*line = readline("smile -m >> < justrandom") was given, after lexer, it should be:
1. { type = 0, lexeme = "smile" }
2. { type = 0, lexeme = "-m" }
3. { type = 4, lexeme = ">>" }
4. { type = 2, lexeme = "<" }
5. { type = 0, lexeme = "justrandom" }
//note: here innlexer we don't deal with invalidate input because simply lexer only focusing
on if we can split the input into catagorized tokens.

This one however will cause error msg:
example3:
*line = readline("'bin/local ./sccript")；we'll get:
"minishell: syntax error: unclosed quote" warning sign, because lexer is supposed to have complete quotes.



