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
    

   
