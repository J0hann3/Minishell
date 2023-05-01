/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:14:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/01 21:40:08 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(char **arg, t_env_info	*env)
{
	char	*str;
	char	*tmp;

	(void)arg;
	(void)env;
	str = getcwd(NULL, 0);
	if (str == NULL)
	{
		g_error = 2;				//Not sure is the good error flag
		ft_write_error("pwd", NULL, strerror(errno));			//Not sure is the good write
		return ;
	}
	tmp = str;
	str = ft_strjoin(str, "\n");
	free(tmp);
	write(1, str, ft_strlen(str));
	free(str);
	free_str(arg);
}