/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:46:19 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/14 04:42:49 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <term.h>
# include <curses.h>
# include "../includes/minishell.h"

size_t	ft_strlen(const char *s);
int		equals(char	*s1, char *s2);
char	*ft_strchr(const char *s, int c);
int		is_meta_character(char c);

int		quotes_not_closed(const char *input);
int		has_argument_right(char *op_ptr);
int		has_argument_left(const char *start_input, char *op_ptr);

int	gget_next_line(char **string);

#endif