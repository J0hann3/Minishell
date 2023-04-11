/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 03:56:01 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/06 23:39:24 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static int	is_alpha(char c) //a changer
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

static int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static bool	is_expandable(const char *input)
{
	return (is_alpha(*input) || *input == '_' || *input == '?');
}

char	*ft_strnjoin(char *s1, char const *s2, size_t size)
{
	char	*res;
	size_t	len;
	size_t	j;

	if (!s1)
		len = size;
	else
		len = ft_strlen(s1) + size;
	j = 0;
	res = malloc(sizeof(char) * len + 1);
	if (res == 0)
		return (NULL);
	while (s1 && s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	len = 0;
	while (len < size)
		res[j++] = s2[len++];
	res[j] = 0;
	free(s1);
	return (res);
}

static int	get_size_of_var(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_alpha(str[i]) || str[i] == '_' || is_digit(str[i]))
			++i;
		else
			return (i);
	}
	return (i);
}

/**
 * @brief search str in the env
 * 
 * @param env char **: terminate by NULL ->where to search variable 
 * @param str char *: Name of variable to find
 * @return int index where is find the variable, if not find return -1
 */
int	ft_getenv(char **env, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (env[i] != NULL)
	{
		j = 0;
		while (env[i][j] != '\0')
		{
			if (env[i][j] == '=' && str[j] == '\0')
				return (i);
			if (str[j] == '\0')
				break ;
			if (str[j] != env[i][j])
				break ;
			++j;
		}
		++i;
	}
	return (-1);
}

int	is_ambigous_redirect(char *input, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		if (*(input - i) == '<' || *(input - i) == '>')
			return (1);
		else if (is_wspace(*(input - i)))
			i++;
		else
			i++;
	}
	return (0);
}

char	*expand(char *input, size_t *i, t_env_info *env_info)
{
	char	*tmp;
	int		env_index;
	size_t	j;
	size_t	size;

	if (*input == '?')
	{
		(*i)++;
		return (ft_itoa(env_info->error));
	}
	size = get_size_of_var(input);
	*i += size;
	tmp = ft_strndup(input, size);
	if (!tmp)
		return (NULL);
	env_index = ft_getenv(env_info->env, tmp);
	free(tmp);
	
	if (env_index == -1)
	{
		if (is_ambigous_redirect(input - 1, *i - size))
			printf("ambigous bizarre -------- \n");//ambigous
		tmp = ft_calloc(1, sizeof(char));
		return (tmp);
	}
	tmp = env_info->env[env_index];
	size = 0;
	while (tmp[size] && tmp[size] != '=')
		size++;
	tmp = ft_strdup(tmp + size + 1);
	return (tmp);
}

char	*expand_dollars(char *input, size_t len, t_env_info *env_info)
{
	size_t	i;
	int		begin_join;
	char	*tmp;
	char	*res;
	
	res = ft_calloc(1, sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	begin_join = 0;
	while (i < len)
	{
		if (input[i] == '\'')
			i += skip_quotes(input + i) + 1;
		else if (input[i] == '$' && is_expandable(input + i + 1))
		{
			if (i > 0)
				res = ft_strnjoin(res, input + begin_join, i - begin_join);
			i++;
			tmp = expand(input + i, &i, env_info);
			if (!tmp)
				return (NULL);
			begin_join = i;
			res = ft_strnjoin(res, tmp, ft_strlen(tmp));
			free(tmp);
		}
		else
			i++;
	}
	if (begin_join != (int)i)
		res = ft_strnjoin(res, input + begin_join, i - begin_join);
	return (res);
}
