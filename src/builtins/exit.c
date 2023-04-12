/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:08:16 by jvigny            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/03/31 21:19:18 by jvigny           ###   ########.fr       */
=======
/*   Updated: 2023/04/11 21:58:07 by qthierry         ###   ########.fr       */
>>>>>>> origin/Quentin
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
void	ft_exit(char **arg, t_env_info *env)			// need to also free tree
{
	unsigned char	erreur;

	if (arg[1] == NULL)
	{
		erreur = env->error;
		free_str(arg);
		free_str(env->env);
		free_tree(env->tree);
		free(env);
		write(2, "exit\n", 5);
		exit(erreur);
	}
	env->error = ft_atouc(arg[1]);
	if (env->error == -1)
	{
		ft_write_error("exit", arg[1], "numeric argument required");
		env->error = 2;
	}
	else if (arg[2] != NULL)
	{
		ft_write_error("exit", NULL, "too many arguments");
		free_str(arg);
		env->error = 1;
		return ;
	}
	erreur = env->error;
	free_tree(env->tree);
	free_str(env->env);
	free_str(arg);
	free(env);
	write(2, "exit\n", 5);
	exit(erreur);
}
