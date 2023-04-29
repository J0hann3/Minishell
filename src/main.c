/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/27 04:03:21 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <termios.h>
#include <curses.h>
#include <term.h>

int	g_error;

int	main(int argc, char *argv[], char *envp[])
{
	char				*input;
	int					ret_err;
	t_env_info			*env;
	const char			*prompt;
	struct termios		termios;

	tcgetattr(STDIN_FILENO, &termios);
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
		tcsetattr(STDIN_FILENO, TCSANOW, &termios);
		input = readline(prompt);
		if (!input)
			break ;
		add_history(input);
		ret_err = syntax_errors(input, env);
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
		env->tree = create_tree(input, env->fds_heredocs, env->len_heredocs);
		if (env->tree == NULL)
			break ;
		explore_tree(env->tree, env, e_empty_new);
		free_tree(&(env->tree));
		env->tree = NULL;
		free(input);
		free(env->fds_heredocs);
		env->fds_heredocs = NULL;
		env->len_heredocs = 0;
	}
	free_str(env->env);
	free(env);
	rl_clear_history();
	write(2, "exit\n", 5);
	return (g_error);
}

// a&& b & (a | c )		Danger
// a| (b&&)
// (      &&)
