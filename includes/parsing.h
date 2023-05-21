/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:46:19 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/22 00:06:56 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "structs.h"

typedef t_instruction	t_instr;

// utils.c
size_t	ft_strlen(const char *s);
bool	eq(const char	*s1, const char *s2);
bool	eqn(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strndup(const char *s, size_t n);
char	*ft_itoa(int n);
void	ft_bzero(void *s, size_t n);

// ft_split.c
t_char	**ft_split_quote(const t_char *string, int *nb_words);

// remove_quotes.c
char	*remove_quotes(t_char *string);

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

// syntax_errors.c
int		syntax_errors(char *input, t_env_info *env);
bool	has_parenthesis_not_closed(const char *input);
bool	quotes_not_closed(const char *input);
bool	has_argument_right(char *op_ptr);
bool	has_argument_left(const char *start_input, char *op_ptr);
bool	is_redirection(char c);
bool	is_parenthesis(char c);

// syntax_utils.c
bool	is_syntax_char(char *string);
bool	is_redirection(char c);
bool	is_parenthesis(char c);
char	*get_error_token(char *input);
bool	is_operator_ok(char **input, char *start_ptr);

bool	is_quote_closed(char **input);

// check_redirections.c
bool	is_redirection_ok(char **input, t_env_info *env);

// ast_utils.c
t_ast	*create_node(const char *command);
enum e_meta_character	get_meta(char *input);
const char				*meta_to_char(enum e_meta_character meta);
size_t					get_command_size(const char *input);

// ast_rec.c
t_ast	*create_tree(char *input, int *fd_heredocs, int len_fd);
t_ast	*create_sub_tree(char **input, t_ast *child, bool *has_error);

// ast.c
t_ast	*create_leaf(const char *input);
t_ast	*create_op_node(char *input);

// parsing_parenthesis.c
void	remove_useless_parenthesis(char **input);
bool	has_error_on_operators_and_parenthesis(const char *input);

void	remove_multiple_wspaces(char *input);
bool	has_error_for_meta(char *input, size_t i);
void	*ft_realloc(void *ptr, size_t prev_size, size_t new_size);
void	*ft_memcpy(void *dest, const void *src, size_t n);

// expand_dollar.c
size_t	get_size_of_var(const char *str);
bool	is_expandable(const char *input);
t_char	*expand_dollars(char *input, size_t len,
			t_env_info *env_info, bool *is_ambigous);
void	print_ambigous_redirect(char *input_redir);

//second_parsing.c
t_instr	*second_parsing(char *input, size_t command_size,
			t_env_info *env_info, int fd_heredocs);

// redirections.c
char	*get_file_name(t_char *input);
bool	open_all_fds(t_instruction *instruc, t_char *input, int fd_heredocs);

// open_files.c
int		read_fd(t_char *input);
int		open_fd(t_char *input);

// get_file_name.c
char	*get_file_name(t_char *input);
void	delete_file_name(t_char *input);

#endif