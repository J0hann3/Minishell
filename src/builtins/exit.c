/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:08:16 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/24 21:32:48 by jvigny           ###   ########.fr       */
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
void	ft_exit(t_instruction *arg, t_env_info *env)			// need to also free tree
{
	unsigned char	erreur;

	if (arg->command[1] == NULL)
	{
		erreur = g_error;
		free_str(arg->command);
		free(arg);
		free_env(env);
		write(2, "exit\n", 5);
		exit(erreur);
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
		free_str(arg->command);
		free(arg);
		g_error = 1;
		return ;
	}
	erreur = g_error;
	free_env(env);
	free_str(arg->command);
	free(arg);
	write(2, "exit\n", 5);
	exit(erreur);
}
