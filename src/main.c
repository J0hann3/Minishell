/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2024/01/10 16:12:12 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <termios.h>
#include <curses.h>
#include <term.h>

int	g_error;

static bool	handle_syntax_errors(char *input, t_env_info *env)
{
	int	ret_error;

	ret_error = syntax_errors(input, env);
	if (ret_error == 2)
	{
		free(input);
		close_fd_heredocs(env);
		g_error = ret_error;
		return (false);
	}
	if (ret_error == 1 || ret_error == 130)
	{
		free(input);
		close_fd_heredocs(env);
		g_error = ret_error;
		return (false);
	}
	return (true);
}

static bool	handle_tree(char *input, t_env_info *env)
{
	env->input = input;
	env->tree = create_tree(input, env->fds_heredocs, env->len_heredocs);
	close_fd_heredocs(env);
	if (env->tree == NULL)
	{
		free(input);
		return (false);
	}
	explore_tree(env->tree, env, e_empty_new);
	free_tree(&(env->tree));
	env->tree = NULL;
	return (true);
}

void	minishell_interactive_loop(t_env_info *env, struct termios *termios)
{
	char	*input;

	while (true)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, termios);
		input = readline("minishell$> ");
		if (!input)
			break ;
		if (!*input)
		{
			free(input);
			continue ;
		}
		add_history(input);
		if (!handle_syntax_errors(input, env))
			continue ;
		if (!handle_tree(input, env))
			continue ;
		free(input);
		env->input = NULL;
	}
	write(1, "exit\n", 5);
}

void	minishell_not_interactive_loop(t_env_info *env)
{
	char	*input;
	size_t	input_len;

	while (true)
	{
		input = get_next_line(STDIN_FILENO);
		if (!input)
			break ;
		input_len = ft_strlen(input);
		if (input_len > 0 && input[input_len - 1] == '\n')
			input[input_len - 1] = 0;
		if (!*input)
		{
			free(input);
			continue ;
		}
		if (!handle_syntax_errors(input, env))
			break ;
		if (!handle_tree(input, env))
			break ;
		free(input);
		env->input = NULL;
		env->does_expand_here = true;
	}
	get_next_line(-1);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_env_info			*env;
	struct termios		termios;

	(void)argv;
	if (argc != 1)
		return (ft_write_error(NULL, NULL, "Wrong number of arguments."), 0);
	g_error = 0;
	env = init_env((const char **)envp);
	if (!env)
		return (1);
	init_signals(env->act);
	tcgetattr(STDIN_FILENO, &termios);
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		minishell_interactive_loop(env, &termios);
	else
		minishell_not_interactive_loop(env);
	free_env(env);
	rl_clear_history();
	return (g_error);
}
