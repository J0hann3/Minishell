/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:21:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 19:53:26 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_option(const char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (0);
	++i;
	while (str[i] != '\0')
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(const char **arg, const t_env_info	*env)
{
	int	i;
	int	option;
	int	check_option;

	(void)env;
	i = 1;
	option = 1;
	check_option = 1;
	while (arg[i] != NULL)
	{
		if (i == 1 || check_option == 1)
		{
			if (is_option(arg[i]) == 1)
			{
				option = 0;
				++i;
				continue ;
			}
		}
		check_option = 0;
		write(1, arg[i], ft_strlen(arg[i]));
		if (arg[++i] != NULL)
			write(1, " ", 1);

	}
	if (option == 1)
		write(1, "\n", 1);
}
