/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:26:26 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/18 17:22:43 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "structs.h"
# include <dirent.h>

typedef struct s_file_list
{
	char	*file_name;
	char	*cursor;
	bool	is_matching;
	bool	is_dir;
}	t_file_list;

// wild_utils.c
char		*get_prefix(const t_char *input, const t_char *start);
char		*get_suffix(const t_char *input, bool *is_end);
bool		is_end_of_single_wildcard(const t_char *input, size_t i);
t_char		*jump_to_pattern_start(const t_char *input, const t_char *start);
t_char		*jump_to_pattern_end(const t_char *input);
bool		is_end_of_pattern(const t_char *input, size_t i);
t_file_list	*init_flist(t_file_list *flist);
void		free_flist(t_file_list *flist);

t_char	*replace_wildcard(t_char *input, t_char **start,
		t_file_list *flist, bool include_hidden);

// replace_input.c
t_char	*test_suffix(t_char *input, t_file_list *flist);
bool	test_first_prefix(t_char *input,
	t_file_list *flist, const t_char *start, bool *include_hidden);

bool		expand_wild(t_char **input);

#endif
