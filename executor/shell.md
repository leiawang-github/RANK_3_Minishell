环境变量是 shell 变量的子集（导出的那一部分）。
所有环境变量一定是某个 shell 变量 export 之后的结果，但不是所有 shell 变量都是环境变量。

- shell 变量特点：
只在当前 shell 内部可见。默认不会传递给子进程。

```bash
VAR=123
echo $VAR      # 在当前 shell 可见
bash           # 启动一个子 shell,也就是子进程
echo $VAR      # 子 shell 看不到（为空）
```

环境变量（env）特点：
是一类特殊的 shell 变量。
通过 export 声明后，才会进入 当前进程的环境块，并且会被继承到子进程。
```bash
export VAR=123
bash           # 启动子 shell
echo $VAR      # 子 shell 能看到
```