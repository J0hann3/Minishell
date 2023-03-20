/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:14:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 18:08:59 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **arg, t_env_info	*env)
{
	char *str;

	(void)arg;
	(void)env;
	str = getcwd(NULL, 0);
	if (str == NULL)
	{
		env->error = 2;				//Not sure is the good error flag
		return (perror("Error"), 1);
	}
	printf("%s\n", str);
	free(str);
	return (0);
}