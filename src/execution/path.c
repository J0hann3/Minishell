/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:59:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/27 16:33:02 by jvigny           ###   ########.fr       */
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

bool	contain_slash(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '/')
			return (true);
		++i;
	}
	return (false);
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
				return (g_error = 2,
					ft_write_error(NULL, NULL, strerror(errno)));
			free(str[0]);
			str[0] = tmp;
			return ;
		}
		++i;
	}
}

char	*access_path(char *path, char *arg)
{
	int	fd;

	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
		{
			fd = open(path, O_DIRECTORY);
			if (fd != -1)
				return (close(fd), g_error = 126, free(path),
					ft_write_error(NULL, arg, "Is a directory"), NULL);
			return (path);
		}
		else
		{
			g_error = 126;
			ft_write_error(NULL, arg, strerror(errno));
			return (free(path), NULL);
		}
	}
	g_error = 127;
	ft_write_error(NULL, arg, strerror(errno));
	return (free(path), NULL);
}

static char	*try_path(char *test_path, char *name, bool *error_mem)
{
	int		fd;
	char	*path;

	if (test_path[ft_strlen(test_path) - 1] != '/')
		path = ft_strjoin3(test_path, "/", name);
	else
		path = ft_strjoin(test_path, name);
	if (path == NULL)
		return (g_error = 2, mem_exh(NULL), *error_mem = true, NULL);
	if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
	{
		fd = open(path, O_DIRECTORY);
		if (fd != -1)
			return (close(fd), free(path), NULL);
		return (path);
	}
	free(path);
	return (NULL);
}

static char	*explore_path(char *name, char *env_path)
{
	char	**var_path;
	char	*path;
	bool	error_mem;
	int		i;

	i = -1;
	error_mem = false;
	var_path = ft_split(env_path, ':');
	if (var_path == NULL)
		return (g_error = 2, mem_exh(NULL), NULL);
	trim_name_var(var_path);
	while (var_path[++i] != NULL)
	{
		path = try_path(var_path[i], name, &error_mem);
		if (!path && error_mem == true)
			return (free_str(var_path), NULL);
		if (path)
			return (free_str(var_path), path);
	}
	free_str(var_path);
	g_error = 127;
	ft_write_error(NULL, name, "command not found");
	return (NULL);
}

char	*find_path_command(char *str, t_env_info *env)
{
	char	*path;
	int		i_path;

	if (!contain_slash(str))
	{
		i_path = ft_getenv(env->env, "PATH");
		if (i_path == -1)
		{
			path = find_absolute_path(str);
			if (path == NULL)
				return (g_error = 2,
					ft_write_error(NULL, NULL, strerror(errno)), NULL);
			return (access_path(path, str));
		}
		return (explore_path(str, env->env[i_path]));
	}
	if (str[0] != '/')
		path = find_absolute_path(str);
	else
		path = ft_strdup(str);
	if (path == NULL)
		return (g_error = 2, ft_write_error(NULL, NULL, strerror(errno)),
			NULL);
	return (access_path(path, str));
}
