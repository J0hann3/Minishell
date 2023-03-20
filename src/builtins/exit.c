/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:08:16 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 18:55:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * 126    A file to be executed was found, but it was not an executable utility
 * 127    A utility to be executed was not found.
 * >128    A command was interrupted by a signal.
 * 
 */


/**
 * @brief 
 * 
 * @param arg 
 * @param env 
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
	else if (arg[2] != NULL)
	{
		env->error = 1;
		return ;
	}
	erreur = ft_atouc(arg[1]);	//code erreur
	if (erreur == -1)
		erreur = 2;				//code erreur
	free_str(arg);
	free_str(env->env);
	free(env);
	exit(erreur);
}

/**
 *  cd | exit ghgf fgjg ; echo $?		->bash: exit: ghgf: numeric argument required  -> 2
 *  
 *  
 * 
 */