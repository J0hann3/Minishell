/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:33:05 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/29 23:10:36 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/**
 * search for valid name, sign = and valid value
 * begin with letters or underscore
 * can only contain letters numbers and underscores
 * return 1 if valid name
*/
static int	is_valid_name(char *str, t_env_info	*env)
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	while (str[i])
	{
		if (is_alpha(str[i]) || str[i] == '_' || str[i] == '=' || is_digit(str[i]))
		{
			if (i == 0 && (is_digit(str[i]) || str[i] == '=' ))
			{
				env->error = 1;
				ft_write_error("export", str, "not a valid identifier");		//need '' around str
				return (0);
			}
			else if (str[i] == '=')
			{
				equal = 1;
				break ;
			}
		}
		else
			return (0);
		++i;
	}
	if (equal == 1)
		return (1);
	else
		return (0);
}

static int	trim_invalid_varible(char **arg, t_env_info	*env)
{
	int	suppr;
	int	i;

	i = 1;
	suppr = -1;
	while (arg[i] != NULL)
	{
		if (!is_valid_name(arg[i], env))
		{
			free(arg[i]);
			arg[i] = NULL;
			++i;
			suppr--;
			continue ;
		}
		++i;
	}
	return (i + suppr);
}

int	is_variable_existing(char **env, char *str, int len_env)
{
	int	i;
	int	j;

	i = 0;
	while (i < len_env)
	{
		j = 0;
		while (str[j] != 0 && env[i][j] != 0)
		{
			if (str[j] != env[i][j])
				break ;
			if (str[j] == '=')
				return (i);
			++j;
		}
		++i;
	}
	return (-1);
}

static void	modifie_var(char **env, char *str, int len)
{
	free(env[len]);
	env[len] = str;
}

static void	add_new_variable(char **arg, char **env, int len_arg, int len_env)
{
	int		i;
	int		elem_add;
	int		var_exist;

	i = 1;
	elem_add = 0;
	while (elem_add < len_arg || arg[i] != NULL)
	{
		// printf("str = %s\n", arg[i]);
		if (arg[i] == NULL)
		{
			++i;
			continue ;
		}
		var_exist = is_variable_existing(env, arg[i], len_env);
		if (var_exist != -1)
		{
			modifie_var(env, arg[i], var_exist);
			++i;
			++elem_add;
		}
		else
		{
			env[len_env] = arg[i];
			++i;
			++len_env;
			++elem_add;
		}
	}
	env[len_env] = NULL;
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

t_env_info	*ft_export(char **arg, t_env_info	*env)
{
	int		len_env;
	int		len_arg;
	char	**new;

	len_arg = trim_invalid_varible(arg, env);
	if (len_arg < 0)
		return (free_arg(arg, len_arg), NULL);
	if (len_arg == 0)
		return (free_arg(arg, len_arg), env);
	len_env = ft_len(env->env);
	if (env->len_env > len_env + len_arg)
	{
		add_new_variable(arg, env->env, len_arg, len_env);
		env->len_env = len_arg + len_env;
		return (env);
	}
	new = malloc(sizeof(char *) * (len_env + len_arg + 1));
	if (new == NULL)
	{
		env->error = 2;
		return (free_arg(arg, len_arg), NULL);
	}
	env->len_env = len_env + len_arg;
	ft_copy(new, env->env);
	add_new_variable(arg, new, len_arg, len_env);
	env->env = new;
	free(env->env);
	free(arg[0]);
	free(arg);
	return (env);
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