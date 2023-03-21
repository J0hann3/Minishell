/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:26:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/21 14:28:05 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * cd //bin/ls		-> bash: cd: //bin/ls: Not a directory
 * cd "		/bin"	-> bash: cd: $'\t/bin/ls': No such file or directory
 * cd /../../..		-> go to /
 * cd /../../../bin -> go to bin
 * 
 * 
 * chdir(changes the current dir)	-> arg = curpath
 * if erreur
 * 		->print error
 * set OLD_PWD
 * set PWD
 * 
 * 
 */

static char	*ft_strjoin_slash(char *s1, char *s2, int add_slash)
{
	char	*res;
	size_t	len;
	size_t	j;

	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2);
	if (add_slash == 1)
		++len;
	res = malloc(sizeof(char) * len + 1);
	if (res == NULL)
		return (0);
	while (s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	if (add_slash == 1)
	{
		res[j] = '/';
		++j;
	}
	len = 0;
	while (s2[len])
	{
		res[j] = s2[len];
		j++;
		len++;
	}
	res[j] = 0;
	return (res);
}

char *find_absolute_path(char *str)
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

static void	update_env(char **env, char *str)
{
	int	i_pwd;
	int	i_old_pwd;

	i_old_pwd = ft_getenv(env, "OLDPWD");
	i_pwd = ft_getenv(env, "PWD");
	if (i_old_pwd != -1)
		free(env[i_old_pwd]);
	if (i_pwd != -1)
	{
		if (i_old_pwd != -1)
			env[i_old_pwd] = env[i_pwd];
		else
			free(env[i_pwd]);
		env[i_pwd] = str;
		return ;
	}
	free(str);
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
int	ft_cd(char **arg, t_env_info	*env)
{
	char	*path;
	// int		len_path;
	// int		len;

	if (arg[1] == NULL)
		return (0);
	if (arg[2] != NULL)
	{
		env->error = 1;		//code error
		ft_write_error("cd", NULL, "too many arguments");
		return (1);
	}
	if (arg[1][0] != '/')
		path = find_absolute_path(arg[1]);
	else
		path = ft_strdup(arg[1]);
	if (path == NULL)
	{
		env->error = 2;		//code error ??
		ft_write_error("cd", arg[1], strerror(errno));
		return(1);		// need to free something
	}
	// len = ft_strlen(path);
	// printf("PATH [%ld] : %s\n", ft_strlen(path), path);
	// len_path = canonical_form(path);
	// add_first_slash(path, len_path, len);
	// path = clean_path(path, len_path);
	if (chdir(path) == -1)
	{
		env->error = 1;		//code error
		ft_write_error("cd", arg[1], strerror(errno));
		return(free(path), 1);		// need to free something
	}
	update_env(env->env, path);
	return (0);
}

// static int	delete_previous_dir(char *str, int index)
// {
// 	int	letter_suppr;
// 	int	i;
// 	int	nb_slash;

// 	nb_slash = 0;
// 	letter_suppr = 0;
// 	i = index;
// 	while (index >= 0)
// 	{
// 		while (index > 0 && str[index] == '\0')
// 			index--;
// 		if (index > 0 && i != index && str[index] == '/')
// 			nb_slash++;
// 		if (nb_slash == 2 || index <= 0)
// 			break ;
// 		str[index] = '\0';
// 		letter_suppr++;
// 		index--;
// 	}
// 	// printf("dot-dot : %d\n", letter_suppr);
// 	return (letter_suppr);
// }

// static int	delete_dot_slash(char *str, int index)
// {
// 	int	letter_suppr;
// 	int	i;

// 	letter_suppr = 0;
// 	i = index;
// 	while (index >= 0)
// 	{
// 		while (index > 0 && str[index] == '\0')
// 			index--;
// 		if (index <= 0 || (i != index && str[index] == '/'))
// 			break ;
// 		str[index] = '\0';
// 		letter_suppr++;
// 		index--;
// 	}
// 	// printf("dot : %d\n", letter_suppr);
// 	return (letter_suppr);
// }

// static int	trim_slash(char *str, int i)
// {
// 	int	letter_suppr;

// 	letter_suppr = 0;
// 	while (i >= 0)
// 	{
// 		if (str[i] == '/')
// 		{
// 			str[i] = '\0';
// 			letter_suppr++;
// 			i--;
// 		}
// 		else
// 			break ;
// 	}
// 	// printf("slash : %d\n", letter_suppr);
// 	return (letter_suppr);
// }

// int	canonical_form(char *str)
// {
// 	int		i;
// 	int		letter_suppr;
// 	int		nb_dot;
// 	int		nb_slash;

// 	i = 0;
// 	letter_suppr = 0;
// 	while (str[i] != '\0')
// 	{
// 		nb_dot = 0;
// 		while (str[i] == '.')
// 		{
// 			++nb_dot;
// 			++i;
// 		}
// 		nb_slash = 0;
// 		while (str[i] != '\0' && str[i] == '/' && str[i + 1] == '/')
// 		{
// 			++nb_slash;
// 			++i;
// 		}
// 		if (nb_slash >= 1)
// 			letter_suppr += trim_slash(str, i - 1);
// 		if (str[i] == '/' || str[i] == '\0')
// 		{
// 			if (str[i] == '\0')
// 				i = i - 1;
// 			if (nb_dot == 2)
// 				letter_suppr += delete_previous_dir(str, i);
// 			else if (nb_dot == 1)
// 				letter_suppr += delete_dot_slash(str, i);
// 			if (str[i + 1] == '\0')
// 			{
// 				++i;
// 				break ;
// 			}
// 		}
// 		++i;
// 	}
// 	return (i - letter_suppr);
// }

// static int	add_first_slash(char *str, int len_path, int real_len)
// {
// 	int	i;
// 	int	j;
	
// 	i = 0;
// 	j = 0;
// 	while (j < len_path)
// 	{
// 		if (str[i] != '\0')
// 		{
// 			if (i - 1 >= 0 && str[i] != '/')
// 			{
// 				str[i - 1] = '/';
// 				len_path--;
// 			}
// 			j++;
// 			break ;
// 		}
// 		++i;
// 	}
// 	if (len_path == 0 && real_len > 1)
// 	{
// 		str[0] = '/';
// 		str[1] = '\0';
// 		len_path = 1;
// 	}
// 	return (len_path);
// }

// static char	*clean_path(char *str, int len_path)
// {
// 	char	*new;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	new = malloc(sizeof(char) * (len_path + 1));
// 	if (new == NULL)
// 		return (free(str), NULL);
// 	while (j < len_path)
// 	{
// 		if (str[i] != '\0')
// 		{
// 			new[j] = str[i];
// 			j++;
// 		}
// 		++i;
// 	}
// 	new[j] = 0;
// 	free(str);
// 	return (new);
// }


