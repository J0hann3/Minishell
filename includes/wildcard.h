/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:26:26 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/02 18:43:12 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_C
# define WILDCARD_C

# include "structs.h"
# include <dirent.h>

char	*expand_wild(char *input);

#endif
