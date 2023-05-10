/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:27:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/10 03:09:31 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	find_for_end_only(t_file_list *flist, char *to_find, int i)
{
	bool	rm_slash;
	size_t	pos_to_compare;

	rm_slash = flist[i].is_dir && to_find[ft_strlen(to_find) - 1] == '/';
	pos_to_compare = ft_strlen(flist[i].cursor) - ft_strlen(to_find) + rm_slash;
	if (ft_strlen(to_find) - rm_slash > ft_strlen(flist[i].cursor)
		|| !eqn(to_find, flist[i].cursor
				+ pos_to_compare, ft_strlen(to_find) - rm_slash))
	{
		flist[i].is_matching = 0;
		flist[i].cursor = NULL;
	}
}

/**
 * @brief finds a pattern for each elemts of the file name list.
 * Fills flist properly if a pattern is found or not.
 * 
 * @param flist 
 * @param to_find 
 * @param is_end 
 */
void	find_pattern_in_fname(t_file_list *flist, char *to_find, bool is_end)
{
	char	*tmp;
	int		i;

	i = 0;
	while (flist[i].file_name)
	{
		if (flist[i].is_matching && ft_strlen(to_find) != 0)
		{
			if (is_end)
				find_for_end_only(flist, to_find, i);
			else
			{
				tmp = ft_strnstr(flist[i].cursor, to_find, ft_strlen(flist[i].cursor));
				if (!tmp || (is_end == true && *(tmp + ft_strlen(to_find)) != '\0'))
				{
					flist[i].is_matching = 0;
					flist[i].cursor = NULL;
				}
				else
					flist[i].cursor = tmp + ft_strlen(to_find);
			}
		}
		i++;
	}
}

char	*test_first_prefix(char *input, t_file_list *flist, const char *start)
{
	char	*to_test;
	char	*tmp;
	int		i;

	i = 0;
	to_test = get_prefix(input, start);
	if (!to_test)
		return (NULL);
	while (flist[i].file_name != NULL)
	{
		tmp = ft_strnstr(flist[i].file_name, to_test, ft_strlen(flist[i].file_name));
		if (!tmp || tmp != flist[i].file_name)
			flist[i].is_matching = 0;
		else
		{
			flist[i].is_matching = 1;
			flist[i].cursor = tmp + ft_strlen(to_test);
		}
		i++;
	}
	free(to_test);
	return (input);
}

char	*test_suffix(char *input, t_file_list *flist)
{
	char	*suffix;
	bool	is_end;

	suffix = get_suffix(input, &is_end);
	if (!suffix)
		return (NULL);
	input += ft_strlen(suffix) + 1;
	find_pattern_in_fname(flist, suffix, is_end);
	free(suffix);
	if (is_end)
		return (NULL);
	return (input);
}

char	*get_file_name_string(t_file_list *flist, bool include_hidden, bool *has_merror)
{
	char	*files_names;
	char	*tmp;
	char	*sep;

	sep = NULL;
	files_names = NULL;
	while (flist && flist->file_name)
	{
		if (flist->is_matching
			&& (include_hidden || (*flist->file_name != '.' && !include_hidden)))
		{
			tmp = ft_strjoin3(files_names, sep, flist->file_name);
			free(files_names);
			if (!tmp)
				return (mem_exh("wildcard"), *has_merror = true, NULL);
			files_names = tmp;
			sep = " ";
		}
		flist++;
	}
	*has_merror = false;
	return (files_names);
}

bool	has_to_include_hidden(const char *input)
{
	if (*input == '\'' || *input == '"')
		input++;
	return (*input == '.');
}

bool	replace_on_input(char **start, char *to_insert, char *pos_pattern)
{
	size_t	size;
	char	*new_ptr;
	char	*tmp;

	size = pos_pattern - *start;
	new_ptr = ft_strndup(*start, size);
	if (!new_ptr)
		return (free(*start), false); // write error
	tmp = ft_strjoin(new_ptr, to_insert);
	free(new_ptr);
	if (!tmp)
		return (free(*start), false); // write error
	pos_pattern = jump_to_pattern_end(pos_pattern);
	new_ptr = ft_strjoin(tmp, pos_pattern);
	free(tmp);
	free(*start);
	if (!new_ptr)
		return (false); // write error
	*start = new_ptr;
	return (true);
}

int	replace_wildcard(char *input, char **start, t_file_list *flist, bool include_hidden)
{
	char	*files_names;
	char	*pat_start;
	char	*pat_end;
	bool	has_merror;
	int		size;
	
	files_names = get_file_name_string(flist, include_hidden, &has_merror);
	if (has_merror)
		return (-1);
	pat_start = jump_to_pattern_start(input, *start);
	pat_end = jump_to_pattern_end(input);
	if (!files_names)
		files_names = ft_strndup(pat_start, pat_end - pat_start);
	if (!files_names)
		return (mem_exh("wildcard"), -1);
	if(!replace_on_input(start, files_names, pat_start))
		return (free(files_names), -1);
	size = ft_strlen(files_names);
	free(files_names);
	return (size);
}

bool	wildcard(char *input, t_file_list *flist, char **start, size_t *i)
{
	int		size;
	char	*tmp;

	tmp = input;
	test_first_prefix(tmp, flist, *start);
	tmp = test_suffix(tmp, flist);
	while (tmp && *tmp == '*')
		tmp = test_suffix(tmp, flist);
	size = replace_wildcard(input, start, flist, has_to_include_hidden(input));
	if (size < 0)
		return (free(flist), false);
	*i += size;
	return (true);
}

bool	expand_wild(char **input)
{
	size_t	i;
	t_file_list	*flist;

	i = 0;
	flist = NULL;
	while ((*input)[i])
	{
		if ((*input)[i] == '"' || (*input)[i] == '\'')
			i += skip_quotes((*input) + i) + 1;
		else if ((*input)[i] == '*')
		{
			if (!flist)
			{
				flist = init_flist(flist);
				if (!flist)
					return (false);
			}
			if (!wildcard((*input) + i, flist, input, &i))
				return (free(flist), false);
		}
		else
			i++;
	}
	free(flist);
	return (true);
}