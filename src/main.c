/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:54:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/31 22:34:24 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../includes/parsing.h"

#include <sys/stat.h>
#include <fcntl.h>



// int main(int argc, char **argv, char **envp)
// {
// 	// t_env_info *env;
// 	// t_instruction	command;
// 	int			i;
// 	// // char		*ligne;
// 	// i = 0;
// 	// argv[argc - 1] = NULL;

// 	// env = init_env((const char **)envp);
// 	// command.command = init_arg((const char **)argv);

// 	int fd = open("test", O_RDWR);
// 	printf("fd:%d\n", fd);
// 	if (fd < 0)
// 		return (perror("error"), 1);
// 	int pid = fork();
// 	if (pid == 0)
// 	{
// 		dup2(fd, STDOUT_FILENO);
// 		printf("fd:%d\n", fd);
// 		printf("test\n");
// 		write(fd, "hello10", 7);
// 		close(fd);
// 		return (0);
// 	}
// 	waitpid(pid, &i, 0);
// 	close(fd);
// 	printf("fd:%d\n", fd);
// 	printf("test\n");
// 	write(fd, "hello", 5);
// 	write(1, "hello5", 6);
// 	// ft_export(argv, env);
// 	// while(1)
// 	// {
// 	// 	env->error = 0;
// 	// 	rl_outstream = stderr;
// 	// 	ligne = readline("minishell$> ");
// 	// 	add_history(ligne);
// 	// 	argv = ft_split(ligne, ' ');
// 	// 	ft_cd(argv, env);
// 	// 	ft_pwd(argv, env);
// 	// 	// free_str(argv);
// 	// }
// 	return (0);
// }


int	main(int argc, char *argv[], char *envp[])
{
	char		*input;
	int			ret_err;
	t_env_info	*env;
	// t_ast		*tree;

	(void)argc;
	(void)argv;
	(void)env;
	input = (char *)1;
	env = init_env((const char **)envp);
	if (env == NULL)
		return (1);
	ret_err = 0;
	while (input != NULL)
	{
		input = readline("minishell$>");
		if (!input)
			break ;
		add_history(input);
		ret_err = syntax_errors(input);
		if (ret_err != 0)
		{
			printf("ERROR:	%d\n",ret_err);
			free(input);
			return (ret_err);
		}
		env->tree = create_tree(input);
		if (env->tree == NULL)
			break ;
		explore_tree(env->tree, env, e_empty, 0);
		free_tree(env->tree);
		env->tree = NULL;
		free(input);
	}
	free_str(env->env);
	free(env);
	rl_clear_history();
	return (ret_err);
}
