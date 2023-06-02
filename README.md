# Minishell
ls | grep e > outfile > outfile1
ls | grep e >> outfile >> outfile1
< infile grep a | grep e > outfile


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

- GOOD TO KNOW :
execve -> mutation de process
‘|&’ is used, command1’s standard error, in addition to its standard output
Redirections are processed in the order they appear, from left to right. 

- godbolt
- learngitbranching

GIT :
	git checkout -b [yourbranchname] ->create branch and change at the same time

	git cherry-pick <Commit1> <Commit2> <...>		-> copy commit where you are

	git rebase -i HEAD~4 ->choose which commit to rebase in which order


valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes

builtins:
◦ echo with option -n							|	Done		ok
◦ pwd with no options							|	Done
◦ env with no options or arguments				|	Done
◦ cd with only a relative or absolute path		|	Done
◦ export with no options						|	Done		ok
◦ unset with no options							|	Done
◦ exit with no options							|	Done


EXIT STATUS :
- he return status of a simple command is its exit status as provided by the waitpid function, or 128+n if the command was terminated by signal n.
- The exit status of a pipeline is the exit status of the last command in the pipeline

- The return status of and and or lists is the exit status of the last command executed in the list

WAITPID :
	1. WIFEXITED(status): child exited normally 
	• WEXITSTATUS(status): return code when child exits
	2. WIFSIGNALED(status): child exited because a signal was not caught 
	• WTERMSIG(status): gives the number of the terminating signal
	3. WIFSTOPPED(status): child is stopped 
	• WSTOPSIG(status): gives the number of the stop signal

non interactive : readline writes a prompt

change _= ???

hereocs creates a .nfs -> bug ?

*		-> need to sort in alphabet order

echo src*/		->src/
echo src*		->src

(echo a)(echo c)

use_heredoc in open_fd not usefull ??

heredocs
redirection
variable$
builtin
*
parsing
|
&&
||
()
env -i et compagnie

a changer :
- ajouter les '' dans le message d'erreur de export invalid identifier

changements : 
- main en interactif
- exit ne print plus 'exit' en interactif
- g_error est set à 0 uniquement après check si on utilise exit
- sep dans les wildcard est maintenant un t_char non-alloc

heredoc en none interactif affiche le mot de fin de heredoc


119 error on mandatory