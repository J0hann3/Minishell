/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/25 17:32:18 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>

int	g_error;

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	int					ret_err;
	int					size;
	t_env_info			*env;
	const char			*prompt;

	(void)argc;
	(void)argv;
	if (argc != 1)
		return (1);
	env = init_env((const char **)envp);
	if (env == NULL)
		return (1);
	init_signals(env->act);
	prompt = "minishell$> ";
	if (!isatty(STDIN_FILENO) || !isatty(STDERR_FILENO))
		prompt = "";
	input = (char *)1;
	ret_err = 0;
	while (input != NULL)
	{
		input = readline(prompt);
		if (!input)
			break ;
		add_history(input);
		int *fds_heredoc = NULL;
		size = 0;
		ret_err = syntax_errors(input, &fds_heredoc, &size); //rajouter la taille size
		if (ret_err == 2)
		{
			printf("ERROR:	%d\n", ret_err);
			free(input);
			continue ;
			// return (ret_err); // leak on return, change to break env->error
		}
		else if (ret_err == 1)
		{
			free(input);
			continue ;
		}
		env->tree = create_tree(input);
		if (env->tree == NULL)
			break ;
		explore_tree(env->tree, env, e_empty_new);
		free_tree(&(env->tree));
		env->tree = NULL;
		free(input);
	}
	free_str(env->env);
	free(env);
	rl_clear_history();
	write(2, "exit\n", 5);
	return (g_error);
}

// unset OLDPWD PWD && env | grep PWD