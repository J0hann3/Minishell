/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 19:00:14 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:30:52 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_ERRORS_H
# define SYNTAX_ERRORS_H
# include "structs.h"

// check_arguments_l_r.c
bool	has_argument_left(const char *start_input, char *op_ptr);
bool	has_argument_right(char *op_ptr);

// check_redirections.c
bool	is_redirection_ok(char **input, t_env_info *env);

// parsing_parenthesis.c
bool	has_closing_parenthesis(char **input, t_env_info *env, char *start_ptr);

// parsing_utils.c
bool	is_operator(const char *c);
bool	is_single_meta(const char *c);
bool	is_double_meta(const char *c);
bool	is_wspace(int c);
bool	is_and_or(const char *input);

// syntax_errors.c
bool	is_quote_closed(char **input);
bool	realloc_heredocs(t_env_info *env);
int		syntax_errors(char *input, t_env_info *env);

// syntax_utils.c
bool	is_syntax_char(char *string);
bool	is_redirection(char c);
bool	is_parenthesis(char c);
char	*get_error_token(char *input);
bool	is_operator_ok(char **input, char *start_ptr);

#endif