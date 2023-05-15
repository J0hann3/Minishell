/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:14:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/13 22:23:35 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(char **arg, t_env_info	*env)
{
	char	*str;

	(void)arg;
	(void)env;
	str = getcwd(NULL, 0);
	if (str == NULL)
	{
		g_error = 2;
		ft_write_error("pwd", NULL, strerror(errno));
		free_str(arg);
		return ;
	}
	write(1, str, ft_strlen(str));
	write(1, "\n", 1);
	free(str);
	free_str(arg);
}
