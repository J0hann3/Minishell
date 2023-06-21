/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:26:26 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/21 15:49:52 by jvigny           ###   ########.fr       */
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

// file_list.c
int			init_one_file(DIR *dirp, t_file_list **flist, size_t size);
t_file_list	*init_flist(t_file_list *flist);
void		free_flist(t_file_list *flist);

// pattern_finder.c
void		find_pattern_in_fname(t_file_list *flist,
				char *to_find, bool is_end);
t_char		*test_suffix(t_char *input, t_file_list *flist, bool *error);
bool		test_first_prefix(t_char *input,
				t_file_list *flist, const t_char *start, bool *include_hidden);

// prefix.c
char		*get_prefix(const t_char *input, const t_char *start);

// replace_input.c
t_char		*get_file_name_string(t_file_list *flist, bool include_hidden);
t_char		*replace_on_input(t_char **start, t_char *to_insert,
				t_char *pat_start, t_char *pat_end);
t_char		*replace_wildcard(t_char *input, t_char **start,
				t_file_list *flist, bool include_hidden);

// suffix.c
char		*get_suffix(const t_char *input, bool *is_end);

// wild_utils.c
bool		is_end_of_single_wildcard(const t_char *input, size_t i);
bool		is_end_of_pattern(const t_char *input, size_t i);
t_char		*jump_to_pattern_start(const t_char *input, const t_char *start);
t_char		*jump_to_pattern_end(const t_char *input);

// wildcard.c
bool		is_ambigous_redirection(t_char *input,
				t_char *start, t_file_list *flist);
bool		wildcard(t_char *input,
				t_file_list *flist, t_char **start, size_t *i);
bool		expand_wild(t_char **input);

#endif
