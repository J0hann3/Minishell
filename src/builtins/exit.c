/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:08:16 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/16 20:17:39 by qthierry         ###   ########.fr       */
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
	if (arg->command[1] == NULL)
	{
		free_str(arg->command);
		free(arg);
		free_env(env);
		if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
			write(2, "exit\n", 5);
		exit((unsigned char)g_error);
	}
	g_error = ft_atouc(arg->command[1]);
	if (g_error == -1)
	{
		ft_write_error("exit", arg->command[1], "numeric argument required");
		g_error = 2;
	}
	else if (arg->command[2] != NULL)
	{
		ft_write_error("exit", NULL, "too many arguments");
		return (free_str(arg->command), free(arg), g_error = 1, (void)0);
	}
	(free_env(env), free_str(arg->command));
	free(arg);
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		write(2, "exit\n", 5);
	exit(g_error);
}
