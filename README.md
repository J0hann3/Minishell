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

export A=123456 && echo $A
>123456

export A=123456 | echo $A
>




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




struct s_instruction
{
	char **command;
	char **fd;
}

s_instruction;

recup this info after 
- infile
- outfile
- error

< read
>write (clear ou create)
>>write (append)

<< char **args


vrai && (faux || vrai) || vrai

union node
{
	struct s_instruction	*pdn_m;
	enum		meta;
};

struct	t_node
{
	t_node	*child1;
	t_node	*child2;
	t_node	*parent;
	unnio node	data;
}





execve -> mutattion de process


GOOD TO KNOW
‘|&’ is used, command1’s standard error, in addition to its standard output
Redirections are processed in the order they appear, from left to right. 

godbolt
learngitbranchingx

git checkout -b [yourbranchname] ->create branch and change at the same time

git cherry-pick <Commit1> <Commit2> <...>		-> copy commit where you are

git rebase -i HEAD~4 ->choose which commit to rebase in which order

revert ??

builtins:
◦ echo with option -n
◦ pwd with no options
◦ env with no options or arguments
◦ cd with only a relative or absolute path
◦ export with no options
◦ unset with no options
◦ exit with no options