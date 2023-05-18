/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:59:33 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/19 00:55:04 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_alpha(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

/**
 * @brief search str in the env
 * 
 * @param env char **: terminate by NULL ->where to search variable 
 * @param str char *: Name of variable to find
 * @return int index where is find the variable, if not find return -1
 */
int	ft_getenv(char **env, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (env[i] != NULL)
	{
		j = 0;
		while (env[i][j] != '\0')
		{
			if (env[i][j] == '=' && str[j] == '\0')
				return (i);
			if (str[j] == '\0')
				break ;
			if (str[j] != env[i][j])
				break ;
			++j;
		}
		++i;
	}
	return (-1);
}
