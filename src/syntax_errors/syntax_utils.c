/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 22:46:50 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 23:06:41 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_syntax_char(char *string)
{
	return (
		*string == '\'' || *string == '"' || *string == '<' || *string == '>'
		|| *string == '(' || *string == ')' || is_operator(string)
	);
}

bool	is_redirection(char c)
{
	return (c == '<' || c == '>');
}

bool	is_parenthesis(char c)
{
	return (c == '(' || c == ')');
}

char	*get_error_token(char *input)
{
	if (!*input)
		return ("syntax error near unexpected token `newline'");
	if (*input == '&' && *(input + 1) == '&')
		return ("syntax error near unexpected token `&&'");
	if (*input == '|' && *(input + 1) == '|')
		return ("syntax error near unexpected token `||'");
	if (*input == '|')
		return ("syntax error near unexpected token `|'");
	if (*input == '<' && *(input + 1) == '<' && *(input + 2) == '<')
		return ("syntax error near unexpected token `<<<'");
	if (*input == '>' && *(input + 1) == '>')
		return ("syntax error near unexpected token `>>'");
	if (*input == '<' && *(input + 1) == '<')
		return ("syntax error near unexpected token `<<'");
	if (*input == '>' && *(input + 1) != '>')
		return ("syntax error near unexpected token `>'");
	if (*input == '<' && *(input + 1) != '<')
		return ("syntax error near unexpected token `<'");
	if (*input == '(')
		return ("syntax error near unexpected token `('");
	if (*input == ')')
		return ("syntax error near unexpected token `)'");
	return ("syntax error near unexpected token `error'");
}

/**
 * @brief A operator considered ok is an operator with valid arguments
 * before (left) and after (right) itself.
 * Forward input to after the right argument.
 * Write error if the arguments are not both valid are not valid.
 * This function can call and return value of : 
 * 		is_quote_closed
 * @param input 
 * @return true 
 * @return false 
 */
bool	is_operator_ok(char **input, char *start_ptr)
{
	bool	boolean;

	boolean = (has_argument_left(start_ptr, *input)
			&& has_argument_right(*input));
	if (is_and_or(*input))
		(*input)++;
	return (boolean);
}
