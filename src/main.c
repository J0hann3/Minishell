/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/22 18:23:59 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_error;

int	main(int argc, char *argv[], char *envp[])
{
	char		*input;
	int			ret_err;
	t_env_info	*env;
	const char *prompt;

	(void)argc;
	(void)argv;
	
	g_error = 0;
	env = init_env((const char **)envp);
	prompt = "minishell$> ";
	if (!isatty(STDIN_FILENO) || !isatty(STDERR_FILENO))
		prompt = "";
	input = (char *)1;
	if (env == NULL)
		return (1);
	ret_err = 0;
	while (input != NULL)
	{
		input = readline(prompt);
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
		explore_tree(env->tree, env, e_empty_new, 0);
		free_tree(&(env->tree));
		env->tree = NULL;
		free(input);
	}
	free_str(env->env);
	ret_err = g_error;
	free(env);
	rl_clear_history();
	write(2, "exit\n", 5);
	return (ret_err);
}
