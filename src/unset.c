/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:38:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/16 14:19:35 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_variable(char **env, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (env[i] != NULL)
	{
		j = 0;
		while (env[i][j] != 0)
		{
			// printf("j : %d	str : %c	env : %c\n", j, str[j],env[i][j]);
			if (env[i][j] == '=' &&str[j] == '\0')
				return (i);
			if (str[j] == '\0')
				break ;
			if (str[j] != env[i][j])
				break ;
			++j;
		}
		// printf("----------------------\n");
		++i;
	}
	return (0);
}

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
		i_unset = is_variable(env, arg[i]);
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