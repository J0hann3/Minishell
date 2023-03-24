/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:46:19 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/24 18:14:28 by jvigny           ###   ########.fr       */
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
bool	is_operator(const char *c);
bool	is_wspace(int c);
bool	is_meta_character(char c);
size_t	skip_quotes(const char *input);
bool	is_and_or(const char *input);
size_t	skip_parenthesis(const char *input);
bool	is_and(const char *input);
bool	is_single_meta(const char *c);
bool	is_double_meta(const char *c);

// syntax_errors
bool	has_parenthesis_not_closed(const char *input);
bool	quotes_not_closed(const char *input);
bool	has_argument_right(char *op_ptr);
bool	has_argument_left(const char *start_input, char *op_ptr);

// ast_utils.c
t_ast	*create_node(const char *command);

// ast.c
t_ast	*create_tree(char *input);

// parsing_parenthesis.c
void	remove_useless_parenthesis(char **input);
bool	has_error_on_operators_and_parenthesis(const char *input);

// chained_list.c
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstadd_back(t_list **lst, void *content);
int		ft_lstsize(t_list *lst);

int		syntax_errors(char *input);
void	remove_multiple_wspaces(char *input);
bool	has_error_for_meta(char *input, size_t i);
void	*ft_realloc(void *ptr, size_t prev_size, size_t new_size);
void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif