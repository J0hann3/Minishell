/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 03:56:01 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/19 00:51:28 by qthierry         ###   ########.fr       */
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

void	print_ambigous_redirect(char *input_redir)
{
	char	*tmp;

	while (*input_redir != '$')
		input_redir++;
	input_redir++;
	tmp = ft_strndup(input_redir - 1, get_size_of_var(input_redir) + 1);
	if (!tmp)
		return (mem_exh("expand dollar"));
	ft_write_error(NULL, tmp, "ambigous redirect");
	free(tmp);
}

bool	is_ambig_redir(char *input, int index)
{
	int	i;

	i = get_size_of_var(input + 1) + 1;
	if (input[i] && !is_wspace(input[i]))
		return (false);
	i = 1;
	while (i < index)
	{
		if (*(input - i) == '>') // change
			return (print_ambigous_redirect(input), true);
		if (*(input - i) == '<')
		{
			if (i + 1 < index && *(input - i - 1) == '<')
				return (false);
			print_ambigous_redirect(input);
			return (true);
		}
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
		if (is_ambig_redir(input - 1, *i - size))
			*is_ambigous = true;
		return (ft_calloc(1, sizeof(char)));
	}
	tmp = env_info->env[env_index];
	j = 0;
	while (tmp[j] && tmp[j] != '=')
		j++;
	tmp = ft_strdup(tmp + j + 1);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0' && is_ambig_redir(input - 1, *i - size))
		*is_ambigous = true;
	return (tmp);
}

// void	set_wspace_to_inter(t_char *tstr)
// {
// 	size_t	i;

// 	i = 0;
// 	while (tstr[i].c)
// 	{
// 		if (is_wspace(tstr[i].c))
// 			tstr[i].is_inter = 1;
// 		i++;
// 	}
// }

bool	is_heredoc(char *pat_start, char *start)
{
	int		i;
	bool 	first_wspace;

	i = 0;
	first_wspace = false;
	if (pat_start > start)
		i--;
	while (pat_start + i >= start)
	{
		if (is_wspace(pat_start[i]))
		{
			first_wspace = true;
			i--;
		}
		else if (pat_start[i] == '>')
			return (false);
		else if (pat_start[i] == '<')
		{
			if (pat_start + i > start && pat_start[i - 1] == '<')
				return (true);
			return (false);
		}
		else if (first_wspace)
			return (false);
		else
			i--;
	}
	return (false);
}

t_char	*expand_dollars(char *input, size_t len, t_env_info *env_info, bool *is_ambigous)
{
	size_t	i;
	int		begin_join;
	char	*tmp;
	t_char	*res;
	t_char	*tmpchar;
	t_char	*tmpchar_1;
	bool	in_double_quotes;

	in_double_quotes = false;
	res = ft_calloc(1, sizeof(t_char));
	if (!res)
		return (mem_exh("dollar expand"), NULL);
	i = 0;
	begin_join = 0;
	while (i < len)
	{
		if (input[i] == '\'' && !in_double_quotes)
			i += skip_quotes(input + i) + 1;
		else if (input[i] == '\"')
		{
			in_double_quotes ^= true;
			i++;
		}
		else if (input[i] == '$' && is_expandable(input + i + 1))
		{
			if (!is_heredoc(input + i, input))
			{
				if (i > 0)
				{
					tmpchar = tchar_njoin(res, input + begin_join, i - begin_join, 1);
					free(res);
					if (!tmpchar)
						return (mem_exh("dollar expand"), NULL);
					res = tmpchar;
				}
				i++;
				tmp = expand(input + i, &i, env_info, is_ambigous);
				if (!tmp)
					return (free(res), mem_exh("dollar expand"), NULL);
				if (*is_ambigous)
					return (free(tmp), free(res), NULL);
				tmpchar = ft_str_to_tchar(tmp, 0);
				free(tmp);
				if (!tmpchar)
					return (free(res), mem_exh("dollar expand"), NULL); // write error
				begin_join = i;
				tmpchar_1 = tchar_join(res, tmpchar);
				free(tmpchar);
				free(res);
				if (!tmpchar_1)
					return (mem_exh("dollar expand"), NULL);
				res = tmpchar_1;
			}
			else
				i++;
		}
		else
			i++;
	}
	if (begin_join != (int)i)
	{
		tmpchar = tchar_njoin(res, input + begin_join, i - begin_join, 1);
		free(res);
		res = tmpchar;
		if (!res)
			mem_exh("dollar expand");
	}
	return (res);
}
