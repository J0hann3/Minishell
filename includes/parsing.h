/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:46:19 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/16 18:12:29 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include "../includes/minishell.h"

# define TOMATE 1 * 4

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// utils.c
size_t	ft_strlen(const char *s);
bool	eq(char	*s1, char *s2);
char	*ft_strchr(const char *s, int c);

// parsing_utils.c
bool	is_wspace(int c);
bool	is_meta_character(char c);
size_t	skip_quotes(const char *input);

bool	quotes_not_closed(const char *input);
bool	has_argument_right(char *op_ptr);
bool	has_argument_left(const char *start_input, char *op_ptr);

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