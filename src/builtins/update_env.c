/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:44:56 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/24 12:45:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_pwd(t_env_info	*env, char *str, int i_old_pwd, int i_pwd)
{
	char	*res;

	if (i_old_pwd != -1)
	{
		res = ft_strjoin("OLD", env->env[i_pwd]);
		free(env->env[i_pwd]);
		if (res == NULL)
			return (g_error = 1, ft_write_error("cd", str, strerror(errno)),
				free(str));
		free(env->env[i_old_pwd]);
		env->env[i_old_pwd] = res;
	}
	else
		free(env->env[i_pwd]);
	env->env[i_pwd] = ft_strjoin("PWD=", str);
	if (env->env[i_pwd] == NULL)
		return (g_error = 1, env->env[i_pwd] = str,
			ft_write_error("cd", str, strerror(errno)), free(str));
	free(str);
}

void	update_env(t_env_info	*env, char *str)
{
	int		i_pwd;
	int		i_old_pwd;
	int		len_env;

	i_old_pwd = ft_getenv(env->env, "OLDPWD");
	i_pwd = ft_getenv(env->env, "PWD");
	if (i_pwd != -1)
		return (update_pwd(env, str, i_old_pwd, i_pwd));
	else
	{
		if (i_old_pwd != -1)
		{
			len_env = ft_len(env->env);
			if (len_env <= 0)
				return (free(str));
			free(env->env[i_old_pwd]);
			env->env[i_old_pwd] = env->env[len_env - 1];
			env->env[len_env - 1] = NULL;
		}
	}
	free(str);
}
