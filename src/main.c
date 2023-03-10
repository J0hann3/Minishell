/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:54:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/10 15:47:48 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	// int i = 0;
	
	// printf("begin ls\n");
	argv[0] = "ls";
	// argv[1] = "< out";
	argv[argc - 1] = NULL;
	env[0] = "TEST=10";
	// ft_env(argv, env);
	printf("	TEST = %s\n",getenv("TEST"));
	printf("	SHELL = %s\n",getenv("SHELL"));
	// int pid = fork();
	// // printf("pid create: %d\n", pid);
	// if (pid == 0)
	// {
	// 	// printf("pid child : %d\n", getpid());
	// 	return (execve("/bin/ls", argv, env));
	// }
	// printf("pid parent: %d\n", getpid());
	// waitpid(pid, &i, WNOHANG);
	// wait(&i);
	// perror("erreur");
	// printf("after ls %d, errno : %s\n", i, 	strerror(i));
	return (0);
}

// struct s_instruction
// {
// 	char **command;
// 	char **fd;
// }

// s_instruction;
// infile
// outfile
// error

// < read
// >write (clear ou create)
// >>write (append)

// << char **args


// vrai && (faux || vrai) || vrai

// union node
// {
// 	struct s_instruction	*pdn_m;
// 	enum		meta;
// };

// t_node
// {
// 	t_node	*child1;
// 	t_node	*child2;
// 	t_node	*parent;
// 	node	command|mate;
	
// }
