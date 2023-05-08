# Minishell

grep e > outfile < infile
grep e < infile > outfile
grep e < infile < infile1
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


- TASK :
quentin : main "parsing + errors" $ENV heredoc "lister fd" signaux "open les fds" *
johanne : execution built-in pipes "find path"

? : $?


execve -> mutattion de process


- GOOD TO KNOW :
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
- Syntax :
	"|" : besoin arguments des deux cotés
	"&&" et "||" : besoin argument des deux cotés
	"<" et ">" droite uniquement
	"<<" et ">>" droite uniquement



expand $ avant le *
$ + signe non accepte dns pour variable export -> remplace rien
$ + variable name -> remplace par la valer de l'env, si n'existe pas, remplace par ''
$ + accepte + non accepte -> remplace et affiche le non accepte a la suite
cas speciaux :
$* -> ne fais rien comme un caractere non accepte basique, puis interprete quand meme l'etoile
$? remplace par la valeur actuelle d'erreur


fds : -2 par defaut, si rien n'a ete ouvert, ou tente
	  -1 si une erreur pendant l'ouverture = jojo ne fais rien
arrete a la premiere erreur d'ouverture pour >, >> et <


// syntax erreur

dans l'ordre gauche a droite, ne print qu'une seul erreur, remonte pas les erreurs

- dans '>' -> parentheses impossible, sinon tout sauf <, renvoie la premiere erreur rencontree
- dans '"' -> rien tant que pas trouve ", si EOF alors double erreur
- dans '(' -> attend << ou " dans l'ordre, suit les regles des deux autres si besoin
- regle de precedence gauche droite pour les erreurs d'operateur sans operant


ERREUR DE MALLOC : <FUNCTIONNALITE> ": memory exausted"

erreur parsing
erreur ambiguous redirect
><test		-> needto write an error

cat <<te"st		->invalid read
don't add in history if ligne empty				|done
echo <<hey $?									|done
non interactive : readline writes a prompt

cat <<fin&& <<fin -> probleme 					| Done

g      && (dfghj) ghj		->need to write error


change _= ???

unset -TEST		->error 2						| Done

echo "exit_code ->$? user ->$USER home -> $HOME"

minishell$> echo <"minishell_tester/test_files/empty" "bonjour       42" 
write : y bonjour 42
echo dfgfd > test (redirige le prompt)

exit 9223372036854775808  ->overflow

 > * 
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( ambiguous redirect)

