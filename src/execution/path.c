/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:59:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/19 01:22:29 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_absolute_path(char *str)
{
	char	*pwd;
	char	*path;
	int		len;
	int		add_slash;

	add_slash = 0;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (NULL);
	len = ft_strlen(pwd);
	if (len != 0 && pwd[len - 1] != '/')
		add_slash = 1;
	path = ft_strjoin_slash(pwd, str, add_slash);
	free(pwd);
	return (path);
}

int	contain_slash(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '/')
			return (1);
		++i;
	}
	return (0);
}

static void	trim_name_var(char **str)
{
	char	*tmp;
	int		i;

	i = 0;
	if (str[0] == NULL)
		return ;
	while (str[0][i] != '\0')
	{
		if (str[0][i] == '=')
		{
			i++;
			tmp = ft_strdup((char *)(str[0] + i));
			if (tmp == NULL)
				return (g_error = 2, ft_write_error(NULL, NULL, strerror(errno)));
			free(str[0]);
			str[0] = tmp;
			return ;
		}
		++i;
	}
}

static char	*explore_path(char *name, char *env_path)
{
	char	**var_path;
	char	*path;
	int		i;
	int		fd;

	i = 0;
	var_path = ft_split(env_path, ':');
	if (var_path == NULL)
		return (NULL);		//now write command not found
	trim_name_var(var_path);
	while (var_path[i] != NULL)
	{
		if (var_path[i][ft_strlen(var_path[i]) - 1] != '/')
			path = ft_strjoin3(var_path[i], "/", name);
		else
			path = ft_strjoin(var_path[i], name);
		if (path == NULL)
		{
			++i;
			continue ;
		}
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
		{
			fd = open(path, O_DIRECTORY);
			if (fd != -1)
				return (close(fd), free_str(var_path), close(fd), NULL);
			return (free_str(var_path), path);
		}
		free(path);
		++i;
	}
	free_str(var_path);
	return (NULL);
}

char	*find_path_command(char *str, t_env_info *env)
{
	char	*path;
	int		i_path;
	int		fd;

	if (contain_slash(str) != 0)
	{
		if (str[0] != '/')
			path = find_absolute_path(str);
		else
			path = ft_strdup(str);
		if (path == NULL)
			return (g_error = 2, ft_write_error(NULL, NULL, strerror(errno)),
				NULL);
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) == 0)
			{
				fd = open(path, O_DIRECTORY);
				if (fd != -1)
					return (close(fd), g_error = 126,
						ft_write_error(NULL, str, "Is a directory"), NULL);
				return (path);
			}
			else
			{
				g_error = 126;
				ft_write_error(NULL, str, strerror(errno));
				return (free(path), NULL);
			}
		}
		g_error = 127;
		ft_write_error(NULL, str, strerror(errno));
		return (free(path), NULL);
	}
	i_path = ft_getenv(env->env, "PATH");
	if (i_path == -1)
	{
		g_error = 127;
		ft_write_error(NULL, str, "command not found");
		return (NULL);
	}
	path = explore_path(str, env->env[i_path]);
	if (path == NULL)
	{
		g_error = 127;
		ft_write_error(NULL, str, "command not found");
	}
	return (path);
}
