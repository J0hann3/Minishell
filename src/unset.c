/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:38:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/16 15:15:42 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(char **arg, char **env)
{
	int	i;
	int	i_unset;
	int	len_env;

	i = 1;
	len_env = ft_len(env);
	while (arg[i] != NULL)
	{
		if (len_env == 0)
			return (0);
		i_unset = ft_getenv(env, arg[i]);
		if (i_unset != 0)
		{
			free(env[i_unset]);
			env[i_unset] = env[len_env - 1];
			env[len_env - 1] = NULL;
			len_env--;
		}
		++i;
	}
	return (0);
}
