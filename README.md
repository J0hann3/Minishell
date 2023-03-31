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

? : $?


Parsing :

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