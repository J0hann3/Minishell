/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:27:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/09 20:12:27 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	find_for_end_only(t_file_list *flist, char *to_find, int i)
{
	bool	rm_slash;

	rm_slash = flist[i].is_dir && to_find[ft_strlen(to_find) - 1] == '/';
	while (*flist[i].cursor
			&& !eqn(flist[i].cursor, to_find,
				ft_strlen(to_find) - rm_slash))
		flist[i].cursor++;
	if (!*flist[i].cursor)
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

char	*get_file_name_string(t_file_list *flist, bool include_hidden)
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
				return (mem_exh("wildcard"), NULL);
			files_names = tmp;
			sep = " ";
		}
		flist++;
	}
	return (files_names);
}

bool	has_to_include_hidden(const char *input)
{
	if (*input == '\'' || *input == '"')
		input++;
	return (*input == '.');
}

bool	replace_on_input(char **start, char *to_insert, size_t pos, size_t size_of_pattern)
{
	size_t	i;
	size_t	j;
	char	*new_ptr;

	printf("taille : %ld\n", ft_strlen(*start) + ft_strlen(to_insert) - size_of_pattern + 1);
	if (!to_insert)
		return (true);
	new_ptr = ft_calloc(ft_strlen(*start) + ft_strlen(to_insert) - size_of_pattern + 1, sizeof(char));
	if (!new_ptr)
		return (free(*start), NULL); //write error mem_exh + ok to free ?
	i = 0;
	j = 0;
	while ((*start)[i])
	{
		if (i + j >= pos && i + j <= ft_strlen(to_insert))
		{
			new_ptr[i + j] = to_insert[j];
			j++;
		}
		else
		{
			new_ptr[i + j] = (*start)[i];
			i++;
		}
	}
	free(*start);
	*start = new_ptr;
	return (false);
}

bool	replace_wildcard(char *input, char **start, t_file_list *flist, bool include_hidden)
{
	char	*files_names;
	char	*pattern_start;
	char	*pat_end;
	
	files_names = get_file_name_string(flist, include_hidden);
	// if (!files_names)
	// 	return (false);
	printf("input deb : '%s'\n", input);
	pattern_start = input;
	pat_end = jump_to_pattern_end(input);
	printf("input apr : '%s'\n", pat_end);
	printf("LA TAILLE : %ld\n", input - *start);
	printf("START %s\n", *start);
	printf("files names : '%s'\n", files_names);
	replace_on_input(start, files_names, input - *start, pat_end - pattern_start);
	return (true);
}

bool	wildcard(char *input, t_file_list *flist, char **start)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = input;
	test_first_prefix(tmp, flist, *start);
	tmp = test_suffix(tmp, flist);
	while (tmp && *tmp == '*')
		tmp = test_suffix(tmp, flist);
	printf("input in wild : '%s'\n", input);
	input = jump_to_pattern_start(input, *start);
	printf("input apres wild : '%s'\n", input);

	if (!replace_wildcard(input, start, flist, has_to_include_hidden(input)))
		return (free(flist), false);
	return (true);
}

char	*expand_wild(char **input)
{
	size_t	i;
	char	*expanded;
	t_file_list	*flist;

	i = 0;
	flist = NULL;
	expanded = NULL;
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
					return (NULL);
			}
			wildcard((*input) + i, flist, input);
			i++;
		}
		else
			i++;
		printf("i : %ld\n", i);
	}
	printf("INPUT A LA FIN : '%s'\n", (*input));
	return (NULL);
}