# Minishell

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

- CODE : 
./minishell
init env
while (exit)
	readline		here doc -> quentin
	\n
	parse

	history
	test PATH
	exec (path, arg, env)


- TASK :
quentin : main "parsing + errors" $ENV heredoc "lister fd" signaux "open les fds" *
johanne : execution built-in pipes "find path"

? : history $?


- STRUCT :
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


- GOOD TO KNOW :
‘|&’ is used, command1’s standard error, in addition to its standard output
Redirections are processed in the order they appear, from left to right. 

godbolt
learngitbranchingx

git checkout -b [yourbranchname] ->create branch and change at the same time

git cherry-pick <Commit1> <Commit2> <...>		-> copy commit where you are

git rebase -i HEAD~4 ->choose which commit to rebase in which order

--trace-children=yes -> flag valgrind go to child process
valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes

revert ??

need to choose when to suppr arg for builtins

builtins:
◦ echo with option -n							|	Done
◦ pwd with no options							|	Done	->if getcwd return NULL, real steal print the current path even if parent folder is rm
																real pwd don't use PWD variable
◦ env with no options or arguments				|	Done
◦ cd with only a relative or absolute path		|	Done
◦ export with no options						|	Done	-> dont works in pipe
◦ unset with no options							|	Done	-> dont works in pipe
◦ exit with no options							|	Done


EXIT STATUS :
- he return status of a simple command is its exit status as provided by the waitpid function, or 128+n if the command was terminated by signal n.

- The exit status of a pipeline is the exit status of the last command in the pipeline

The return status of and and or lists is the exit status of the last command executed in the list


- Each command in a multi-command pipeline, where pipes are created, is executed in its own subshell, which is a separate process


if value is not given it's assigned a null string
variable value : null string is a valid value
