/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:54:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/16 16:19:55 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	argv[0] = "echo=10";
	argv[argc - 1] = NULL;

	envp = init_env((const char **)envp);
	argv = init_env((const char **)argv);
	cd(argv, envp);
	// env(argv, envp);
	// printf("--------------------------------------------------------------\n");
	// unset(argv, envp);
	// printf("--------------------------------------------------------------\n");
	// env(argv, envp);
	free_str(envp);
	free_str(argv);
	// free(envp);
	// printf("--------------\n");
	// echo(argv, envp);
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
