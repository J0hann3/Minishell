/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:26:26 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/09 20:01:45 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_C
# define WILDCARD_C

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
char		*get_prefix(const char *input, const char *start);
char		*get_suffix(const char *input, bool *is_end);
bool		is_end_of_single_wildcard(const char *input, size_t i);
char		*jump_to_pattern_start(const char *input, const char *start);
char		*jump_to_pattern_end(const char *input);
bool		is_end_of_pattern(const char *input, size_t i);
t_file_list	*init_flist(t_file_list *flist);

char		*expand_wild(char **input);

#endif
