/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:08:16 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 19:57:17 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	ft_exit(char **arg, t_env_info *env)
{
	unsigned char	erreur;

	if (arg[1] == NULL)
	{
		free_str(arg);
		free_str(env->env);
		erreur = env->error;
		free(env);
		exit(erreur);
	}
	env->error = ft_atouc(arg[1]);	//code erreur
	if (env->error == -1)
	{
		ft_write_error("exit", arg[1], "numeric argument required");
		env->error = 2;				//code erreur
	}
	else if (arg[2] != NULL)
	{
		ft_write_error("exit", NULL, "too many arguments");
		env->error = 1;
		return ;
	}
	erreur = env->error;
	free_str(arg);
	free_str(env->env);
	free(env);
	exit(erreur);
}

/**
 *  cd | exit ghgf fgjg ; echo $?		->bash: exit: ghgf: numeric argument required  -> 2
 *  cd | exit 6465 fgjg ; echo $?		->bash: exit: fgjg: too many arguments  -> 1
 */


