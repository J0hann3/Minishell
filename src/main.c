/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/11 20:05:28 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../includes/parsing.h"

int	main(int argc, char *argv[], char *envp[])
{
	char			*input;
	int				ret_err;
	t_env_info		*env;
	t_ast			*tree;

	(void)argc;
	(void)argv;
	input = (char *)1;
	env = init_env((const char **)envp);
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
		tree = create_tree(input);
		explore_tree(tree, env, e_empty, 0);
		free_tree(tree);
		free(input);
	}
	free_str(env->env);
	free(env);
	rl_clear_history();
	return (ret_err);
}
