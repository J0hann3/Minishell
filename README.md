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


quentin : main "parsing + errors" $ENV heredoc "lister fd" signaux "open les fds" *
johanne : execution built-in pipes "find path"

? : history $?


Parsing :

- Syntax :
	"|" : besoin arguments des deux cotés
	"&&" et "||" : besoin argument des deux cotés
	"<" et ">" droite uniquement
	"<<" et ">>" droite uniquement

tests :
	echo u&&(ls&&ls)s	-> erreur de parsing
	()					-> '\0'
	(())				-> '\0'
	( )					-> '\0'
	(ls)				-> 'ls'
	((ls))				-> 'ls'
	()ls				-> '\0' 'ls'
	()ls()				-> '\0' 'ls' '\0'
	()l()s				-> '\0' 'l' '\0' 's'
	echo o&&&ls			-> 'echo o && &ls'
	echo o|||ls			-> erreur de parsing
	()()()				-> '\0' '\0' '\0'
	&|&