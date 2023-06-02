/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 03:56:01 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:15:22 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_dol(char *input, int *i,
			t_env_info *env_info, bool *is_ambigous);

static bool	is_heredoc(char *pat_start, char *start)
{
	int		i;
	bool	first_wspace;

	i = 0;
	first_wspace = false;
	if (pat_start > start)
		i--;
	while (pat_start + i >= start)
	{
		if (is_wspace(pat_start[i]) && --i > 0)
			first_wspace = true;
		else if (pat_start[i] == '>' || first_wspace)
			return (false);
		else if (pat_start[i] == '<')
		{
			if (pat_start + i > start && pat_start[i - 1] == '<')
				return (true);
			return (false);
		}
		else
			i--;
	}
	return (false);
}

/**
 * @brief Returns -1 on alloc error, 0 on ambigous, 1 on non-ambigous redirect
 * 
 * @param input 
 * @param i 
 * @param res 
 * @param env_info 
 * @return int 
 */
static int	join_on_expand(char *input, int *i,
	t_char **res, t_env_info *env_info)
{
	char	*tmp;
	t_char	*tmpchar;
	t_char	*tmpchar_1;
	bool	is_ambigous;

	is_ambigous = false;
	tmp = expand_dol(input + *i, i, env_info, &is_ambigous);
	if (!tmp)
		return (free(*res), mem_exh("dollar expand"), -1);
	if (is_ambigous)
		return (free(tmp), free(*res), 0);
	tmpchar = ft_str_to_tchar(tmp, 0);
	free(tmp);
	if (!tmpchar)
		return (free(*res), mem_exh("dollar expand"), -1);
	tmpchar_1 = tchar_join(*res, tmpchar);
	free(tmpchar);
	free(*res);
	if (!tmpchar_1)
		return (mem_exh("dollar expand"), -1);
	*res = tmpchar_1;
	return (1);
}

enum e_var {
	i,
	begin_join,
	in_double_quotes,
	ret
};

/**
 * @brief WTF? Ask the norminette.
 * Returns -1 on alloc error, 0 on ambigous, 1 on non-ambigous redirect
 * 
 * @param input 
 * @param v 
 * @param res 
 * @param env_info 
 * @return int 
 */
static int	test_for_dollar(char *input,
			int **v, t_char **res, t_env_info *env_info)
{
	t_char	*tmpchar;

	if (input[(*v)[i]] == '\'' && !(*v)[in_double_quotes])
		(*v)[i] += skip_quotes(input + (*v)[i]);
	else if (input[(*v)[i]] == '\"')
		(*v)[in_double_quotes] ^= true;
	else if (input[(*v)[i]] == '$' && is_expandable(input + (*v)[i] + 1)
		&& !is_heredoc(input + (*v)[i], input))
	{
		if ((*v)[i] > 0)
		{
			tmpchar = tchar_njoin(*res, input
					+ (*v)[begin_join], (*v)[i] - (*v)[begin_join], 1);
			free(*res);
			if (!tmpchar)
				return (mem_exh("dollar expand"), -1);
			*res = tmpchar;
		}
		(*v)[i]++;
		(*v)[ret] = join_on_expand(input, &(*v)[i], res, env_info);
		if ((*v)[ret] <= 0)
			return ((*v)[ret]);
		(*v)[begin_join] = (*v)[i]--;
	}
	return ((*v)[i]++, 1);
}

t_char	*expand_dollars(char *input, size_t len,
			t_env_info *env_info, bool *is_ambigous)
{
	int		*v;
	t_char	*res;
	t_char	*tmpchar;

	v = (int [4]){0, 0, 0, 0};
	*is_ambigous = false;
	res = ft_calloc(1, sizeof(t_char));
	if (!res)
		return (mem_exh("dollar expand"), NULL);
	while (v[i] < (int)len)
	{
		v[ret] = test_for_dollar(input, &v, &res, env_info);
		if (v[ret] <= 0)
			return (*is_ambigous = (v[ret] == 0), NULL);
	}
	if (v[begin_join] != (int)v[i])
	{
		tmpchar = tchar_njoin(res, input
				+ v[begin_join], v[i] - v[begin_join], 1);
		free(res);
		res = tmpchar;
		if (!res)
			mem_exh("dollar expand");
	}
	return (res);
}
