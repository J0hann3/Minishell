/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:33:05 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/13 22:26:45 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		trim_invalid_varible(char **arg);
int		modifie_var(char **arg, char **env, int len_arg);
void	add_new_variable(char **arg, char **env, int len_arg, int len_env);

int	ft_len(char **str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] != NULL)
	{
		++i;
	}
	return (i);
}

static void	ft_copy(char **dest, char **src)
{
	int	i;

	i = 0;
	while (src[i] != NULL)
	{
		dest[i] = src[i];
		++i;
	}
}

static void	free_arg(char **arg, int len)
{
	int	i;

	i = 0;
	while (i < len || arg[i] != NULL)
	{
		free(arg[i]);
		++i;
	}
	free(arg);
}

void	ft_export(char **arg, t_env_info	*env)
{
	int		len_env;
	int		len_arg;
	char	**new;

	len_arg = trim_invalid_varible(arg);
	len_arg = modifie_var(arg, env->env, len_arg);
	if (len_arg < 0 || len_arg == 0)
		return (free_arg(arg, len_arg));
	len_env = ft_len(env->env);
	if (env->len_env > len_env + len_arg)
	{
		add_new_variable(arg, env->env, len_arg, len_env);
		env->len_env = len_arg + len_env;
		free(arg[0]);
		free(arg);
		return ;
	}
	new = ft_calloc((len_env + len_arg + 1), sizeof(char *));
	if (new == NULL)
	{
		g_error = 2;
		ft_write_error("export", NULL, "memory exausted");
		return (free_arg(arg, len_arg));
	}
	env->len_env = len_env + len_arg;
	ft_copy(new, env->env);
	add_new_variable(arg, new, len_arg, len_env);
	free(env->env);
	env->env = new;
	free(arg[0]);
	free(arg);
}

/**
 * export			->declare -x all the variable 		->not sure need to do
 * exportNAME=10	->not a error because tempory variable
 * export ""		->not valid name
 * export NAME		->not add to env BUT trigger no error
 * export NAME=		-> add to env with no value
 * export NAME====12	->value is '===12'
 * if NAME already exist modifie the last value with the now one
*/