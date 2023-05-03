/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:26:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/03 16:32:52 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	canonical_form(char *str);

static void	update_env(t_env_info	*env, char *str)
{
	int		i_pwd;
	int		i_old_pwd;
	int		len_env;
	char	*res;

	i_old_pwd = ft_getenv(env->env, "OLDPWD");
	i_pwd = ft_getenv(env->env, "PWD");
	if (i_pwd != -1)
	{
		if (i_old_pwd != -1)		//update oldpwd
		{
			res = ft_strjoin("OLD", env->env[i_pwd]);
			free(env->env[i_pwd]);
			if (res == NULL)
				return (ft_write_error("cd", str, strerror(errno)), free(str),
					g_error = 1, (void)0);
			free(env->env[i_old_pwd]);
			env->env[i_old_pwd] = res;
		}
		else
			free(env->env[i_pwd]);
		env->env[i_pwd] = ft_strjoin("PWD=", str);
		if (env->env[i_pwd] == NULL)
			return (ft_write_error("cd", str, strerror(errno)), g_error = 1,
				env->env[i_pwd] = str, (void)0);
		free(str);
		return ;
	}
	else
	{
		if (i_old_pwd != -1)
		{
			len_env = ft_len(env->env);
			if (len_env <= 0)
				return ;
			free(env->env[i_old_pwd]);
			env->env[i_old_pwd] = env->env[len_env - 1];
			env->env[len_env - 1] = NULL;
		}
	}
	free(str);
}

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

static int	add_first_slash(char *str, int len_path, int real_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < len_path)
	{
		if (str[i] != '\0')
		{
			if (i - 1 >= 0 && str[i] != '/')
			{
				str[i - 1] = '/';
				len_path--;
			}
			j++;
			break ;
		}
		++i;
	}
	if (len_path == 0 && real_len > 1)
	{
		str[0] = '/';
		str[1] = '\0';
		len_path = 1;
	}
	return (len_path);
}

static char	*clean_path(char *str, int len_path)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (str == NULL)
		return (NULL);
	new = malloc(sizeof(char) * (len_path + 1));
	if (new == NULL)
		return (free(str), NULL);
	while (j < len_path)
	{
		if (str[i] != '\0')
		{
			new[j] = str[i];
			j++;
		}
		++i;
	}
	new[j] = 0;
	free(str);
	return (new);
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
	int		len_path;
	int		len;

	if (check_arg(arg) == 0)
		return (g_error = 1, (void)0);
	if (arg[1][0] != '/')
		path = find_absolute_path(arg[1]);
	else
		path = ft_strdup(arg[1]);
	if (path == NULL)
	{
		g_error = 2;
		ft_write_error("cd", arg[1], strerror(errno));
		return (free_str(arg));
	}
	len = ft_strlen(path);
	if (chdir(path) == -1)
	{
		g_error = 1;
		ft_write_error("cd", arg[1], strerror(errno));
		return (free_str(arg), free(path));
	}
	len_path = canonical_form(path);
	len_path = add_first_slash(path, len_path, len);
	path = clean_path(path, len_path);
	if (path == NULL)
	{
		g_error = 2;
		ft_write_error("cd", arg[1], strerror(errno));
		return (free_str(arg));
	}
	update_env(env, path);
	free_str(arg);
}
