task：from int main(int argc, char **argv) -> catagorized tokens

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


