/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:14:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 20:03:42 by jvigny           ###   ########.fr       */
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
		ft_write_error("pwd", NULL, strerror(errno));			//Not sure is the good write
		return (1);
	}
	write(1, str, ft_strlen(str));
	write(1, "\n", 1);
	free(str);
	return (0);
}