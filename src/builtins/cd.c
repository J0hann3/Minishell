/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:26:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/24 12:46:01 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_env(t_env_info	*env, char *str);
int		canonical_form(char *str);

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

	i = -1;
	j = 0;
	while (j < len_path)
	{
		if (str[++i] != '\0')
		{
			if (i - 1 >= 0 && str[i] != '/')
			{
				str[i - 1] = '/';
				len_path--;
			}
			j++;
			break ;
		}
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

static void	change_env(t_env_info *env, char *path, int len, char **arg)
{
	int	len_path;

	len_path = canonical_form(path);
	len_path = add_first_slash(path, len_path, len);
	path = clean_path(path, len_path);
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
	int		len;

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
	len = ft_strlen(path);
	if (chdir(path) == -1)
	{
		ft_write_error("cd", arg[1], strerror(errno));
		return (g_error = 1, free_str(arg), free(path));
	}
	change_env(env, path, len, arg);
	free_str(arg);
}
