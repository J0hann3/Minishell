/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 03:56:01 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/21 17:27:33 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

bool	has_space(const char *string)
{
	while (*string)
	{
		if (is_wspace(*string))
			return (true);
		else
			string++;
	}
	return (false);
}

bool	is_ambigous_redirect(char *input, int index)
{
	int	i;

	i = 1;
	while (i < index)
	{
		if (*(input - i) == '<' || *(input - i) == '>')
			return (true);
		else if (is_wspace(*(input - i)))
			i++;
		else
			return (false);
	}
	return (false);
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
		return (ft_itoa(g_error));
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
		{
			// printf("ambigous -------- \n");//ambigous
			g_error = 1; // ambigous error to change
		}
		return (ft_calloc(1, sizeof(char)));
	}
	tmp = env_info->env[env_index];
	j = 0;
	while (tmp[j] && tmp[j] != '=')
		j++;
	tmp = ft_strdup(tmp + j + 1);
	if (is_ambigous_redirect(input - 1, *i - size) && (!*tmp || has_space(tmp)))
		g_error = 1;
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
				return (free(res), NULL);
			if (g_error == 4)
				return (free(tmp), res);				//error on ambigous to change
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
