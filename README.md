# Minishell

~ == variable HOME in env

cd ~ with HOME empty -> go nowhere
if HOME is unset -> find a other way to go home

grep e > outfile < infile
grep e < infile > outfile
grep e < infile < infile1
ls | grep e > outfile > outfile1
ls | grep e >> outfile >> outfile1
< infile grep a | grep e > outfile


./minishell
init env
while (exit)
	readline		here doc -> quentin
	\n
	parse

	history
	test PATH
	exec (path, arg, env)





quentin : main "parsing + errors" $ENV heredoc "lister fd" signaux "open les fds" *
johanne : execution built-in pipes "find path"

? : history $?













GOOD TO KNOW
‘|&’ is used, command1’s standard error, in addition to its standard output
Redirections are processed in the order they appear, from left to right. 

godbolt