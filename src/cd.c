/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:26:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/18 15:25:47 by jvigny           ###   ########.fr       */
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

char *find_absolute_path(char *str)
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

static int	delete_previous_dir(char *str, int index)
{
	int	letter_suppr;
	int	nb_slash;

	nb_slash = 0;
	letter_suppr = 0;
	while (index > 0)
	{
		while (index > 0 && str[index] == '\0')
			index--;
		if (str[index] == '/')
			nb_slash++;
		str[index] = '\0';
		letter_suppr++;
		index--;
		if (nb_slash == 2)
			break ;
	}
	// printf("letter suppr dir : %d\n", letter_suppr);
	return (letter_suppr);
}

static int	delete_dot_slash(char *str, int index)
{
	int	letter_suppr;

	letter_suppr = 0;
	while (index > 0)
	{
		while (index > 0 && str[index] == '\0')
			index--;
		if (str[index] == '/')
		{
			str[index] = '\0';
			letter_suppr++;
			break ;
		}
		str[index] = '\0';
		letter_suppr++;
		index--;
	}
	// printf("letter suppr dot : %d\n", letter_suppr);
	return (letter_suppr);
}

static int	trim_slash(char *str, int i)
{
	int	letter_suppr;

	letter_suppr = 0;
	while (i > 0)
	{
		if (str[i] == '/')
		{
			str[i] = '\0';
			letter_suppr++;
			i--;
		}
		else
			break ;
	}
	return (letter_suppr);
}

int	canonical_form(char *str)
{
	int		i;
	int		letter_suppr;
	int		nb_dot;
	int		nb_slash;

	i = 0;
	letter_suppr = 0;
	while (str[i] != '\0')
	{
		nb_dot = 0;
		while (str[i] == '.')
		{
			++nb_dot;
			// printf("%c", str[i]);
			++i;
		}
		nb_slash = 0;
		while (str[i] == '/' && str[i + 1] == '/')
		{
			++nb_slash;
			++i;
		}
		if (nb_slash >= 1)
			letter_suppr += trim_slash(str, i - 1);
		if (str[i] == '/' || str[i] == '\0')
		{
			if (nb_dot == 2)
				letter_suppr += delete_previous_dir(str, i - 1);
			else if (nb_dot == 1)
				letter_suppr += delete_dot_slash(str, i - 1);
		}
		// printf("%c", str[i]);
		++i;
	}
	// printf("\nsuppr : %d	len : %d\n", letter_suppr, i);
	return (i - letter_suppr);
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
	int		len_path;
	int		i;

	(void)env;
	if (arg[1] == NULL)
		return (0);
	if (arg[2] != NULL)
		return (1);
	if (arg[1][0] != '/')
	{
		path = find_absolute_path(arg[1]);
		len_path = canonical_form(path);
	}
	else
		return (1);


	
	i = 0;
	int	j = 0;
	printf("len_path : %d\n", len_path);
	while (j < len_path)
	{
		if (path[i] != '\0')
		{
			printf("%c", path[i]);
			j++;
		}
		++i;
	}
	printf("\n");
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

// /mnt/nfs/homes/jvigny/Documents/42/Minishell////../Minishell/./../Minishell/
