/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:14:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/10 15:43:23 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int pwd(char **arg, char **env)
{
	char *str;

	(void)arg;
	(void)env;
	str = getcwd(NULL, 0);
	if (str == NULL)
		return (perror("Error"), 1);
	printf("%s\n", str);
	free(str);
	return (0);
}