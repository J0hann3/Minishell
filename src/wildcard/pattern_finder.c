/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_finder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 17:21:00 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/21 15:48:15 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	find_for_end_only(t_file_list *flist, char *to_find, int i)
{
	bool	rm_slash;
	size_t	pos_to_compare;
	size_t	len_to_find;

	len_to_find = ft_strlen(to_find);
	rm_slash = (flist[i].is_dir && to_find[len_to_find - 1] == '/');
	pos_to_compare = ft_strlen(flist[i].cursor) - len_to_find + rm_slash;
	if (len_to_find - rm_slash > ft_strlen(flist[i].cursor)
		|| !eqn(to_find, flist[i].cursor
			+ pos_to_compare, len_to_find - rm_slash))
	{
		flist[i].is_matching = 0;
		flist[i].cursor = NULL;
	}
}

/**
 * @brief finds a pattern for each elemts of the file name list.
 * Fills flist properly if a pattern is found or not.
 * @param flist 
 * @param to_find 
 * @param is_end 
 */
void	find_pattern_in_fname(t_file_list *flist, char *to_find, bool is_end)
{
	char	*tmp;
	int		i;

	i = 0;
	if (ft_strlen(to_find) == 0)
		return ;
	while (flist[i].file_name)
	{
		if (is_end && flist[i].is_matching)
			find_for_end_only(flist, to_find, i);
		else if (flist[i].is_matching)
		{
			tmp = ft_strnstr(
					flist[i].cursor, to_find, ft_strlen(flist[i].cursor));
			if (!tmp)
			{
				flist[i].is_matching = 0;
				flist[i].cursor = NULL;
			}
			else
				flist[i].cursor = tmp + ft_strlen(to_find);
		}
		i++;
	}
}

t_char	*test_suffix(t_char *input, t_file_list *flist, bool *error)
{
	char	*suffix;
	bool	is_end;

	*error = false;
	suffix = get_suffix(input, &is_end);
	if (!suffix)
		return (*error = true, NULL);
	input += ft_strlen(suffix) + 1;
	find_pattern_in_fname(flist, suffix, is_end);
	free(suffix);
	if (is_end)
		return (NULL);
	return (input);
}

bool	test_first_prefix(t_char *input,
	t_file_list *flist, const t_char *start, bool *include_hidden)
{
	char	*to_test;
	char	*tmp;
	int		i;

	i = 0;
	to_test = get_prefix(input, start);
	if (!to_test)
		return (false);
	*include_hidden = (*to_test == '.');
	while (flist[i].file_name != NULL)
	{
		tmp = ft_strnstr(
				flist[i].file_name, to_test, ft_strlen(flist[i].file_name));
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
	return (true);
}
