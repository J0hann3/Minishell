/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:27:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/21 15:49:30 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_w_redirection(t_char *pat_start, t_char *start)
{
	int	i;

	i = 0;
	if (pat_start > start)
		i--;
	while (pat_start + i >= start)
	{
		if (is_wspace(pat_start[i].c))
			i--;
		else if (pat_start[i].c == '>' && pat_start[i].is_inter)
			return (true);
		else if (pat_start[i].c == '<' && pat_start[i].is_inter)
		{
			if (pat_start + i > start && pat_start[i - 1].c == '<'
				&& pat_start[i - 1].is_inter)
				return (false);
			return (true);
		}
		else
			return (false);
	}
	return (false);
}

static bool	is_w_heredoc(t_char *pat_start, t_char *start)
{
	int		i;
	bool	fst_wspc;

	i = 0;
	fst_wspc = false;
	if (pat_start > start)
		i--;
	while (pat_start + i >= start)
	{
		if (is_wspace(pat_start[i].c))
		{
			i--;
			fst_wspc = true;
		}
		else if ((pat_start[i].c == '>' && pat_start[i].is_inter) || fst_wspc)
			return (false);
		else if (pat_start[i].c == '<' && pat_start[i].is_inter)
		{
			return (pat_start + i > start && pat_start[i - 1].c == '<'
				&& pat_start[i - 1].is_inter);
		}
		else
			i--;
	}
	return (false);
}

bool	is_ambigous_redirection(t_char *input,
		t_char *start, t_file_list *flist)
{
	t_char	*pat_start;
	char	*tmp;
	int		i;
	int		nb_matching;

	pat_start = jump_to_pattern_start(input, start);
	if (!is_w_redirection(pat_start, start))
		return (false);
	i = 0;
	nb_matching = 0;
	while (flist[i].file_name)
	{
		if (flist[i].is_matching)
			nb_matching++;
		i++;
	}
	if (nb_matching > 1)
	{
		jump_to_pattern_end(input)->c = '\0';
		tmp = ft_tchar_to_str(pat_start);
		ft_write_error(NULL, tmp, "ambigous redirect");
		free(tmp);
		return (true);
	}
	return (false);
}

bool	wildcard(t_char *input, t_file_list *flist, t_char **start, size_t *i)
{
	t_char	*tmp;
	bool	include_hidden;
	bool	error;

	tmp = input;
	if (is_w_heredoc(jump_to_pattern_start(input, *start), *start))
		return (*i = (jump_to_pattern_end(input) - *start), true);
	if (!test_first_prefix(tmp, flist, *start, &include_hidden))
		return (false);
	tmp = test_suffix(tmp, flist, &error);
	if (tmp == NULL && error == true)
		return (false);
	while (tmp && tmp->c == '*')
	{
		tmp = test_suffix(tmp, flist, &error);
		if (tmp == NULL && error == true)
			return (false);
	}
	if (is_ambigous_redirection(input, *start, flist))
		return (false);
	tmp = replace_wildcard(input, start, flist, include_hidden);
	if (tmp == NULL)
		return (false);
	*i = tmp - *start;
	return (true);
}

bool	expand_wild(t_char **input)
{
	size_t		i;
	t_file_list	*flist;

	i = 0;
	flist = NULL;
	while ((*input)[i].c)
	{
		if (((*input)[i].c == '"' || (*input)[i].c == '\'')
			&& (*input)[i].is_inter == true)
			i += skip_quotes_tchar((*input) + i) + 1;
		else if ((*input)[i].c == '*')
		{
			if (!flist)
				flist = init_flist(flist);
			if (!flist)
				return (false);
			if (!wildcard((*input) + i, flist, input, &i))
				return (free_flist(flist), false);
		}
		else
			i++;
	}
	free_flist(flist);
	return (true);
}
