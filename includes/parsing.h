/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:46:19 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/15 17:48:03 by qthierry         ###   ########.fr       */
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

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// utils.c
size_t	ft_strlen(const char *s);
int		equals(char	*s1, char *s2);
char	*ft_strchr(const char *s, int c);

// parsing_utils.c
int		is_wspace(int c);
int		is_meta_character(char c);
size_t	skip_quotes(char *input);

int		quotes_not_closed(const char *input);
int		has_argument_right(char *op_ptr);
int		has_argument_left(const char *start_input, char *op_ptr);

// ast_utils.c
t_ast	*ast_new_node(char *command);

// ast.c
char	**parse_args(char *input);
t_ast	*create_tree(char *input);

// chained_list.c
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstadd_back(t_list **lst, void *content);
int		ft_lstsize(t_list *lst);

#endif