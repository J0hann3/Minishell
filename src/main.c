/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:54:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/22 20:04:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char **argv, char **envp)
{
	t_env_info *env;
	t_instruction	command;
	int			i;
	// char		*ligne;
	i = 0;
	argv[argc - 1] = NULL;

	env = init_env((const char **)envp);
	command.command = init_arg((const char **)argv);

	exec(&command, env);
	exit(env->error);
	// ft_export(argv, env);
	// while(1)
	// {
	// 	env->error = 0;
	// 	rl_outstream = stderr;
	// 	ligne = readline("minishell$> ");
	// 	add_history(ligne);
	// 	argv = ft_split(ligne, ' ');
	// 	ft_cd(argv, env);
	// 	ft_pwd(argv, env);
	// 	// free_str(argv);
	// }
	
	// free_str(env->env);
	// free(env);
	return (0);
}
