/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/16 15:59:08 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	int					ret_err;
	t_env_info			*env;
	// t_list				*here_list;
	struct sigaction	action;
	struct sigaction	act_ign;

	if (argc != 1)
		return (1);
	init_signals(action, act_ign);
	(void)argv;
	input = (char *)1;
	env = init_env((const char **)envp);
	if (env == NULL)
		return (1);
	ret_err = 0;
	while (input != NULL)
	{
		input = readline("minishell$> ");
		if (!input)
			break ;
		add_history(input);
		ret_err = syntax_errors(input);
		if (ret_err == 2)
		{
			printf("ERROR:	%d\n", ret_err);
			free(input);
			return (ret_err);
		}
		else if (ret_err == 1)
		{
			free(input);
			continue ;
		}
		// if (get_here_list(here_list, input) < 1)
		// 	return (2);
		action.sa_handler = SIG_DFL;
		env->tree = create_tree(input);
		if (env->tree == NULL)
			break ;
		explore_tree(env->tree, env, e_empty, 0);
		action.sa_handler = crtl_c_interactive;
		free_tree(&(env->tree));
		env->tree = NULL;
		free(input);
	}
	free_str(env->env);
	free(env);
	rl_clear_history();
	write(1, "exit\n", 5);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return (0); //TODO : return env->error
}
