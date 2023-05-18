/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 17:18:01 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/19 00:51:28 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_char	*append_fname(t_char *files_names, char *file_name, t_char *sep)
{
	t_char	*tmp;

	tmp = tchar_join(files_names, sep);
	free(files_names);
	if (!tmp)
		return (free(sep), mem_exh("wildcard"), NULL);
	files_names = tchar_njoin(tmp, file_name, ft_strlen(file_name), 0);
	free(tmp);
	if (!files_names)
		return (mem_exh("wildcard"), NULL);
	return (files_names);
}

t_char	*get_file_name_string(t_file_list *flist, bool include_hidden)
{
	t_char	*files_names;
	t_char	sep[2];

	sep[0] = (t_char){0, 0};
	sep[1] = (t_char){0, 0};
	files_names = ft_calloc(1, sizeof(t_char));
	if (!files_names)
		return (mem_exh("wildcard"), NULL);
	while (flist->file_name)
	{
		if (flist->is_matching && (include_hidden
				|| (*flist->file_name != '.' && !include_hidden)))
		{
			files_names = append_fname(files_names, flist->file_name, sep);
			if (!files_names)
				return (NULL);
			sep[0] = (t_char){' ', 0};
		}
		flist++;
	}
	return (files_names);
}

t_char	*replace_on_input(t_char **start, t_char *to_insert,
		t_char *pat_start, t_char *pat_end)
{
	size_t	size;
	t_char	*new_ptr;
	t_char	*tmp;

	size = pat_start - *start;
	new_ptr = ft_tcharndup(*start, size);
	if (!new_ptr)
		return (mem_exh("wildcard"), NULL);
	tmp = tchar_join(new_ptr, to_insert);
	free(new_ptr);
	if (!tmp)
		return (mem_exh("wildcard"), NULL);
	size = ft_tchar_len(tmp);
	new_ptr = tchar_join(tmp, pat_end);
	free(tmp);
	if (!new_ptr)
		return (mem_exh("wildcard"), NULL);
	free(*start);
	*start = new_ptr;
	return (new_ptr + size);
}

t_char	*replace_wildcard(t_char *input, t_char **start,
		t_file_list *flist, bool include_hidden)
{
	t_char	*files_names;
	t_char	*pat_start;
	t_char	*pat_end;

	files_names = get_file_name_string(flist, include_hidden);
	if (!files_names)
		return (NULL);
	pat_start = jump_to_pattern_start(input, *start);
	pat_end = jump_to_pattern_end(input);
	if (!files_names[0].c)
		return (free(files_names), pat_end);
	pat_end = replace_on_input(start, files_names, pat_start, pat_end);
	free(files_names);
	return (pat_end);
}
