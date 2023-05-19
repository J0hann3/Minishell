/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 18:35:52 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/19 18:38:06 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOCS_H
# define HEREDOCS_H
# include "structs.h"

//heredocs.c
int		do_here_docs(char *input, t_env_info *env_info);

// prompt_here.c
char	*get_random_name(void);
bool	expand_heredocs(int *fd_in, t_env_info *env_info);
int		prompt_here(char *ender, int fd_w, t_env_info *env);

// random_name.c
char	*get_random_name(void);

#endif
