/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:04:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/11 21:58:07 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_env(char **arg, t_env_info	*env)
{
	int	i;

	(void)arg;
	i = 0;
	while (env->env[i] != NULL)
	{
		write(1, env->env[i], ft_strlen(env->env[i]));
		write(1, "\n", 1);
		i++;
	}
	free_str(arg);
}
