/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:26:26 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/08 20:47:16 by qthierry         ###   ########.fr       */
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
}	t_file_list;

char	*expand_wild(char *input);

#endif
