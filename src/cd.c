/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:26:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/16 18:18:45 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strjoin(char *s1, char *s2, int add_slash)
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

char *find_absolute_path(char **env, char *str)
{
	char	*pwd;
	char	*path;
	int		len;
	int		add_slash;

	add_slash = 0;
	pwd = getcwd(NULL, 0);
	if (pwd == 0)
		return (NULL);
	len = ft_strlen(pwd);
	if (len != 0 && pwd[len - 1] != '/')
		add_slash = 1;
	path = ft_strjoin (pwd, str, add_slash);
	free(pwd);
	return (path);
}

char *canonical_form(char *str)
{
	int		i;
	int		len_path;
	int		one_point;
	int		two_point;
	int		more_point;

	i = 0;
	one_point = 0;
	two_point = 0;
	two_point = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '.' )
		{
			if (more_point != 1)
			{
				if (two_point == 1)
					more_point = 0;
				else if (one_point == 1)
				{
					two_point = 1;
					one_point = 0;
				}
				else if (one_point == 0)
					one_point = 1;
			}
		}
		
		
		++i;
	}
	return (str);
}

/**
 * @brief 
 * 
 * begin with / is an absolute path
 * else relative path
 * 
 * @param arg char ** first arg -> cd second arg ->Path more -> ERROR
 * @param env 
 * @return int 
 */
int	cd(char **arg,char **env)
{
	char	*path;
	int		i;

	i = 0;
	if (arg[1] == NULL)
		return (0);
	if (arg[2] != NULL)
		return (1);
	if (arg[1][i] != '/')
		path = find_absolute_path(env, arg[1]);
		path = canonical_form(path);
		
		
	printf("%s\n", path);
	free(path);
	return (0);
}

/**
 * cd //bin/ls		-> bash: cd: //bin/ls: Not a directory
 * cd "		/bin"	-> bash: cd: $'\t/bin/ls': No such file or directory
 * cd /../../..		-> go to /
 * cd /../../../bin -> go to bin
 * 
 */