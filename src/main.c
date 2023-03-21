/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:54:04 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/21 17:59:38 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <readline/readline.h>
#include <readline/history.h>


int main(int argc, char **argv, char **envp)
{
	t_env_info *env;
	char		*ligne;
	argv[argc - 1] = NULL;

	env = init_env((const char **)envp);
	argv = init_arg((const char **)argv);
	printf("ACCESS: %d\n",access(argv[1], F_OK));
	// ft_export(argv, env);
	// while(1)
	// {
	// 	env->error = 0;
	// 	ligne = readline("minishell$> ");
	// 	add_history(ligne);
	// 	argv = ft_split(ligne, ' ');
	// 	ft_export(argv, env);
	// 	if (env->error == 0)
	// 		ft_env(argv, env);
		// free_str(argv);
	// }
	// ft_exit(argv, env);
	// free_str(env->env);
	// free(env);
	return (0);
}
