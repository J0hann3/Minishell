/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/14 20:10:44 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	int					ret_err;
	t_env_info			*env;
	struct sigaction 	action;
	struct sigaction 	act_ign;

	(void)argc;
	(void)argv;
	init_signals(action, act_ign);
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
		env->tree = create_tree(input);
		if (env->tree == NULL)
			break ;
		explore_tree(env->tree, env, e_empty, 0);
		free_tree(&(env->tree));
		env->tree = NULL;
		free(input);
	}
	free_str(env->env);
	free(env);
	rl_clear_history();
	write(1, "exit\n", 5);
	return (0); //TODO : return env->error
}
