/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:38:02 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/15 18:56:24 by jvigny           ###   ########.fr       */
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
		while (str[j] != 0 && env[i][j] != 0)
		{
			if (str[j] == '=')
				return (i);
			if (str[j] != env[i][j])
				break ;
			++j;
		}
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
		i_unset = is_variable(env, arg[i]);
		printf("index: %d\n", i_unset);
		if (i_unset != 0)
		{
			printf("free : [%d]%s	change : [%d]%s/n", i_unset, env[i_unset], len_env - 1, env[len_env - 1]);
			free(env[i_unset]);
			env[i_unset] = env[len_env - 1];
			env[len_env - 1] = NULL;
			len_env--;
		}
		++i;
	}
	return (0);
}