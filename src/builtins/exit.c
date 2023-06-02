/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:08:16 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/02 18:22:34 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 126    A file to be executed was found, but it was not an executable utility
 * 127    A utility to be executed was not found.
 * >128    A command was interrupted by a signal.
 */

/**
 * @brief exit on arg otherwise on env error code
 * 
 * @param arg char *: value to exit
 * @param env char **: code error modifie or use to exit
 */
void	ft_exit(t_instruction *arg, t_env_info *env)
{
	bool	error;

	if (arg->command[1] == NULL)
	{
		(free_instructions(arg), free_env(env));
		if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
			write(2, "exit\n", 5);
		get_next_line(-1);
		exit((unsigned char)g_error);
	}
	g_error = ft_atouc(arg->command[1], &error);
	if (error)
	{
		ft_write_error("exit", arg->command[1], "numeric argument required");
		g_error = 2;
	}
	else if (arg->command[2] != NULL)
		return (ft_write_error("exit", NULL, "too many arguments"),
			g_error = 1, free_str(arg->command));
	free_instructions(arg);
	free_env(env);
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		write(2, "exit\n", 5);
	get_next_line(-1);
	exit(g_error);
}
