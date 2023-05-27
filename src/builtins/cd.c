/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:26:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/27 10:25:53 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_arg(char **arg)
{
	if (arg[1] == NULL)
		return (free_str(arg), 0);
	if (arg[2] != NULL)
	{
		ft_write_error("cd", NULL, "too many arguments");
		return (free_str(arg), 0);
	}
	return (1);
}

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

static void	update_env(t_env_info	*env, char *str)
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

static void	change_env(t_env_info *env, char **arg)
{
	char *path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		ft_write_error("cd", arg[1], strerror(errno));
		return (g_error = 2, (void)0);
	}
	update_env(env, path);
}

/**
 * @brief 
 * 
 * begin with / is an absolute path
 * else relative path
 * 
 * @param arg char ** first arg -> cd second arg ->Path more -> ERROR
 * @param env 
 * @return int sucess ->0 else 1
 */
void	ft_cd(char **arg, t_env_info	*env)
{
	char	*path;

	if (check_arg(arg) == 0)
		return (g_error = 1, (void)0);
	if (arg[1][0] != '/')
		path = find_absolute_path(arg[1]);
	else
		path = ft_strdup(arg[1]);
	if (path == NULL)
	{
		ft_write_error("cd", arg[1], strerror(errno));
		return (g_error = 2, free_str(arg));
	}
	if (chdir(path) == -1)
	{
		ft_write_error("cd", arg[1], strerror(errno));
		return (g_error = 1, free_str(arg), free(path));
	}
	change_env(env, arg);
	free_str(arg);
}
