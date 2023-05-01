/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 03:56:01 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/01 17:36:24 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_expandable(const char *input)
{
	return (is_alpha(*input) || *input == '_' || *input == '?');
}

size_t	get_size_of_var(const char *str)
{
	size_t	i;

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

/**
 * @brief Returns the expanded $ variable, first input character is the very next
 * character after $
 * Forward the i index by the size of the variable
 * 
 * @param input 
 * @param i 
 * @param env_info 
 * @param is_ambigous 
 * @return char* 
 */
char	*expand(char *input, size_t *i, t_env_info *env_info, bool *is_ambigous)
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
			*is_ambigous = true; // ambigous error to change
		return (ft_calloc(1, sizeof(char)));
	}
	tmp = env_info->env[env_index];
	j = 0;
	while (tmp[j] && tmp[j] != '=')
		j++;
	tmp = ft_strdup(tmp + j + 1);
	if (is_ambigous_redirect(input - 1, *i - size) && (!*tmp || has_space(tmp)))
		*is_ambigous = true;
	return (tmp);
}

char	*expand_dollars(char *input, size_t len, t_env_info *env_info, bool *is_ambigous)
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
			tmp = expand(input + i, &i, env_info, is_ambigous);
			if (!tmp)
				return (free(res), NULL);
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
