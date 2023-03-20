/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:54:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 18:46:16 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_env_info *env;
	argv[0] = "exit";
	argv[argc - 1] = NULL;
	
	env = init_env((const char **)envp);
	argv = init_arg((const char **)argv);
	// printf("%s\n", strerror(127));
	// cd(argv, env);
	// pwd(argv, env);
	// // env(argv, envp);
	// // printf("--------------------------------------------------------------\n");
	// // unset(argv, envp);
	// // printf("--------------------------------------------------------------\n");
	// // env(argv, envp);
	
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
	ft_exit(argv, env);
	return (0);
}
