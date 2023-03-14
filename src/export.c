/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:33:05 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/14 19:28:48 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_alpha(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

static int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

/**
 * search for valid name, sign = and valid value
 * begin with letters or underscore
 * can only contain letters numbers and underscores
 * return 1 if invalid name
*/
static int	is_valid_name(char *str)
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
				return (0);
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

// static int	ft_error(void)
// {
// 	return (1);
// }

static int	ft_len(char **str)
{
	int	i;

	i = 0;
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

int	trim_invalid_varible(char **arg)
{
	int	suppr;
	int	i;

	i = 0;
	suppr = 0;
	while (arg[i] != NULL)
	{
		printf("%s is valid : %d\n", arg[i], is_valid_name(arg[i]));
		if (!is_valid_name(arg[i]))
		{
			// error = ft_error();
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

char	**export(char **arg, char **env)
{
	int		i;
	int		len_env;
	int		len_arg;
	int		elem_add;
	char	**new;

	i = 0;
	elem_add = 0;
	len_arg = trim_invalid_varible(arg);
	if (len_arg < 0)		//not sure it's can happened
		return (NULL);	//attention because arg can fill with NULL in the middle
	if (len_arg == 0)
		return (env);
	len_env = ft_len(env);
	printf("len_malloc: %d	len_env: %d		len_arg: %d\n", len_env + len_arg + 1, len_env, len_arg);
	new = malloc(sizeof(char *) * (len_env + len_arg + 1));
	if (new == NULL)
		return (NULL);		//dont no what to free
	ft_copy(new, env);
	free(env);
	while (elem_add < len_arg || arg[i] != NULL)	//not sure of the condition
	{
		printf("str = %s\n", arg[i]);
		if (arg[i] == NULL)
		{
			++i;
			continue ;
		}
		new[len_env] = arg[i];
		++i;
		++len_env;
		++elem_add;
	}
	new[len_env] = NULL;
	free(arg);
	return (new);
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