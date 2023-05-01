/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:38:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/01 21:40:37 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief free variable without ralloc the env
 * 
 * @param arg variable to free
 * @param env 
 * @return int: 0 on sucess
 */
void	ft_unset(char **arg, t_env_info	*env)
{
	int	i;
	int	i_unset;
	int	len_env;

	i = 1;
	len_env = ft_len(env->env);
	while (arg[i] != NULL)
	{
		if (len_env == 0)
			return ;
		i_unset = ft_getenv(env->env, arg[i]);
		if (i_unset != -1)
		{
			free(env->env[i_unset]);
			env->env[i_unset] = env->env[len_env - 1];
			env->env[len_env - 1] = NULL;
			len_env--;
		}
		++i;
	}
	free_str(arg);
	return ;
}
