/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:38:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/13 22:23:12 by jvigny           ###   ########.fr       */
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
			return (free_str(arg));
		i_unset = ft_getenv(env->env, arg[i]);
		if (i_unset != -1)
		{
			free(env->env[i_unset]);
			env->env[i_unset] = env->env[len_env - 1];
			env->env[len_env - 1] = NULL;
			len_env--;
		}
		else if (i == 1 && arg[i][0] == '-' && arg[i][1] != '\0')
			return (g_error = 2, ft_write_error("unset", arg[i], "invalid option"), free_str(arg));
		++i;
	}
	free_str(arg);
	return ;
}
