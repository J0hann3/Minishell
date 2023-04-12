/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/12 16:42:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	int					ret_err;
	t_env_info			*env;

	(void)argc;
	(void)argv;
	init_signals();
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
		free_tree(&(env->tree));
		env->tree = NULL;
		free(input);
	}
	free_str(env->env);
	free(env);
	rl_clear_history();
	return (ret_err);
}
