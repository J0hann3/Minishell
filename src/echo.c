/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:21:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/10 15:48:19 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void echo(char **arg, char **env)
{
	int	i;
	int	option;

	(void)env;
	i = 1;
	option = 1;
	while (arg[i] != NULL)
	{
		printf("%s", arg[i]);
		i++;
	}
	if (option == 1)
		printf("\n");
}