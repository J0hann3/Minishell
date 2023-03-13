/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 15:33:05 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/11 17:26:46 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * search for valid name, sign = and valid value
 * 
 * return len or 0 if invalid name
*/
static int	is_valid_name()
{
	
}

static int	ft_error()
{
	return (1);
}

static int	ft_len(char **str)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
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

char	**export(const char **arg, char **env)
{
	int	i;
	int	j;
	int	suppr;
	char	**new;
	int	error;

	i = 1;
	error = 0;
	suppr = 0;
	while (arg[i] != NULL)
	{
		if (is_valid_name(arg[i]) == 0)
		{
			error = ft_error();
			++i;
			free(arg[i]);
			arg[i] = NULL;
			suppr--;
			continue ;
		}
		
		

		++i;
	}
	j = ft_len(arg);
	new = malloc(sizeof(char *) * (j + i + suppr + 1));		//attention if malloc < env
	if (new == NULL)
		return (NULL);
	ft_copy(new, arg);
	i = 0;
	while (suppr < 0 || arg[i] != NULL)
	{
		if (arg[i] == NULL)
		{
			++suppr;
			++i;
			continue ;
		}
		dest[j] = arg[i];
		++i;
		++j;
	}
	dest[i] = NULL;
	return (dest);
}

/**
 * export			->declare -x all the variable 		->not sure need to do
 * exportNAME=10	->not a error because tempory variable
 * export ""		->not valid name
 * export NAME		->not add to env
 * export NAME=		-> add to env with no value
 * export NAME+=12		->add 12 after the last value
 * export NAME====12	->value is '===12'
 * if NAME already exist modifie the last value with the now one
*/