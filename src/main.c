/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:54:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/24 18:05:39 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "../includes/parsing.h"

#include <sys/stat.h>
#include <fcntl.h>



// int main(int argc, char **argv, char **envp)
// {
// 	t_env_info *env;
// 	t_instruction	command;
// 	int			i;
// 	// char		*ligne;
// 	i = 0;
// 	argv[argc - 1] = NULL;

// 	env = init_env((const char **)envp);
// 	command.command = init_arg((const char **)argv);

// 	exec(&command, env);
// 	free_str(env->env);
// 	exit(env->error);
// 	free(env);
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


int	main(int argc, char *argv[], char *env[])
{
	char	*input;
	int		ret_err;

	(void)argc;
	(void)argv;
	(void)env;
	input = (char *)1;
	ret_err = 0;
	while (input != NULL)
	{
		input = readline("minishell$>");
		if (!input)
			break ;
		add_history(input);
		ret_err = syntax_errors(input);
		create_tree(input);
		free(input);
	}
	rl_clear_history();
	return (ret_err);
}
