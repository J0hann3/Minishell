/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:06:50 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/19 16:50:43 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * search for valid name, sign = and valid value
 * begin with letters or underscore
 * can only contain letters numbers and underscores
 * return 1 if valid name
*/
static int	is_valid_name(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_alpha(str[i]) || str[i] == '_' || str[i] == '='
			|| is_digit(str[i]))
		{
			if (i == 0 && (is_digit(str[i]) || str[i] == '='))
			{
				ft_write_error("export", str, "not a valid identifier");
				return (g_error = 1, false);
			}
			else if (str[i] == '=')
				return (true);
		}
		else
		{
			g_error = 1 + (str[i] == '-' && i == 0);
			ft_write_error("export", str, "not a valid identifier");
			return (false);
		}
		++i;
	}
	return (false);
}

int	trim_invalid_varible(char **arg)
{
	int	suppr;
	int	i;

	i = 1;
	suppr = -1;
	while (arg[i] != NULL)
	{
		if (!is_valid_name(arg[i]))
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

static int	is_variable_existing(char **env, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (env[i] != NULL)
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

int	modifie_var(char **arg, char **env, int len_arg)
{
	int		i;
	int		elem;
	int		modif;
	int		var_exist;

	i = 1;
	elem = 0;
	modif = 0;
	while (elem < len_arg || arg[i] != NULL)
	{
		if (arg[i] == NULL && ++i)
			continue ;
		var_exist = is_variable_existing(env, arg[i]);
		if (var_exist != -1)
		{
			free(env[var_exist]);
			env[var_exist] = arg[i];
			arg[i] = NULL;
			modif++;
		}
		i++;
		elem++;
	}
	return (len_arg - modif);
}

void	add_new_variable(char **arg, char **env, int len_arg, int len_env)
{
	int		i;
	int		elem_add;
	int		var_exist;

	i = 1;
	elem_add = 0;
	while (elem_add < len_arg || arg[i] != NULL)
	{
		if (arg[i] == NULL && ++i)
			continue ;
		var_exist = is_variable_existing(env, arg[i]);
		if (var_exist != -1)
		{
			free(env[var_exist]);
			env[var_exist] = arg[i++];
		}
		else
			env[len_env++] = arg[i++];
		++elem_add;
	}
	env[len_env] = NULL;
}
