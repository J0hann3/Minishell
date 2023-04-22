/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/22 17:13:28 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// syntax erreur

// dans l'ordre gauche a droite, ne print qu'une seul erreur, remonte pas les erreurs

// - dans '>' -> parentheses impossible, sinon tout sauf <, renvoie la premiere erreur rencontree
// - dans '"' -> rien tant que pas trouve ", si EOF alors double erreur
// - dans '(' -> attend << ou " dans l'ordre, suit les regles des deux autres si besoin
// - regle de precedence gauche droite pour les erreurs d'operateur sans operant
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


static char *get_error_token(char *input)
{
	if (!*input)
		return (ft_strdup("syntax error near unexpected token `newline'"));
	if (*input == '&' && *(input + 1) == '&')
		return (ft_strdup("syntax error near unexpected token `&&'"));
	if (*input == '|' && *(input + 1) == '|')
		return (ft_strdup("syntax error near unexpected token `||'"));
	if (*input == '|')
		return (ft_strdup("syntax error near unexpected token `|'"));
	if (*input == '<' && *(input + 1) == '<' && *(input + 2) == '<')
		return (ft_strdup("syntax error near unexpected token `<<<'"));
	if (*input == '>' && *(input + 1) == '>')
		return (ft_strdup("syntax error near unexpected token `>>'"));
	if (*input == '<' && *(input + 1) == '<')
		return (ft_strdup("syntax error near unexpected token `<<'"));
	if (*input == '>' && *(input + 1) != '>')
		return (ft_strdup("syntax error near unexpected token `>'"));
	if (*input == '<' && *(input + 1) != '<')
		return (ft_strdup("syntax error near unexpected token `<'"));
	if (*input == '(')
		return (ft_strdup("syntax error near unexpected token `('"));
	if (*input == ')')
		return (ft_strdup("syntax error near unexpected token `)'"));
	return (ft_strdup("syntax error near unexpected token `error'"));
}

/**
 * @brief Checks if the given quote is closed, forward input to the
 * closing quote, and set error_token if closing quote does not exist
 * 
 * @param input 
 * @param error_token 
 * @return true 
 * @return false 
 */
bool	is_quote_closed(char **input, char **error_token)
{
	char		quote;
	const char	*error_quote = "unexpected EOF while looking for matching \
`\"'\nminishell: syntax error: unexpected end of file";

	quote = **input;
	(*input)++;
	while (**input)
	{
		if (**input == quote)
			return (true);
		(*input)++;
	}
	*error_token = ft_strdup(error_quote);
	return (false);
}

/**
 * @brief Check if the redirection has a valid argument
 * Forward input at the last charcater of the argument.
 * If no valid argument, set error_token to the redirection.
 * This function call heredoc open if encounterd.
 * This function can call and return value of : 
 * 		is_quote_closed
 * @param input 
 * @param error_token 
 * @return true 
 * @return false 
 */
bool	is_redirection_ok(char **input, char **error_token)
{
	char	*start;

	(*input)++;
	if (**input == '<' || **input == '>')
		(*input)++;
	while (is_wspace((**input)))
		(*input)++;
	start = *input;
	while (**input && !is_wspace(**input) && !is_operator((*input))
			&& !is_parenthesis(**input) && !is_redirection(**input))
	{
		if ((**input == '\'' || **input == '"')
			&& !is_quote_closed(input, error_token))
				return (false);
		(*input)++;
	}
	if ((*input) == start)
	{
		*error_token = get_error_token((*input));
		return (false);
	}
	else
		(*input)--;
	return (true);
}

/**
 * @brief A operator considered ok is an operator with valid arguments
 * before (left) and after (right) itself.
 * Forward input to after the right argument.
 * Set error_token if the arguments are not both valid are not valid.
 * This function can call and return value of : 
 * 		is_quote_closed
 * @param input 
 * @param error_token 
 * @return true 
 * @return false 
 */
bool	is_operator_ok(char **input, char **error_token, char *start_ptr)
{
	return (has_argument_left(start_ptr, *input, error_token)
		&& has_argument_right(*input, error_token));
}

/**
 * @brief Checks if the given parenthesis is closed, forward input to the
 * closing parenthesis, and set error_token if closing parenthesis does not exist
 * 
 * @param input 
 * @param error_token 
 * @return true 
 * @return false 
 */
bool	has_closing_parenthesis(char **input, char **error_token)
{
	const char *error_parenth = "unexpected EOF while looking for matching \
`('\nminishell: syntax error: unexpected end of file";
	(*input)++;
	if (**input == ')')
	{
		*error_token = ft_strdup("syntax error near unexpected token `)'");
		return (false);
	}
	while (**input)
	{
		if (**input == ')')
			return (true);
		else if (**input == '(' && !has_closing_parenthesis(input, error_token))
			return (false);
		else if ((**input == '"' || **input == '"')
				&& !is_quote_closed(input, error_token))
			return (false);
		else if ((**input == '<' || **input == '>')
				&& !is_redirection_ok(input, error_token))
			return (false);
		(*input)++;
	}
	*error_token = ft_strdup(error_parenth);
	return (false);
}

/**
 * @brief
 * return -1 on unexpected error
 * return 0 if syntax error and set the error_token to the syntax error chars
 * return 1 if the syntax is ok
 * 
 * @param input 
 * @param error_token 
 * @return int 
 */
int	check_syntax_at(char **input, char **error_token, char *start_ptr)
{
	if (**input == '\'' || **input == '"')
		return (is_quote_closed(input, error_token));
	if (**input == '<' || **input == '>')
		return (is_redirection_ok(input, error_token));
	if (is_operator(*input))
		return (is_operator_ok(input, error_token, start_ptr));
	if (**input == '(')
		return (has_closing_parenthesis(input, error_token));
	return (-1);
}


/**
 * @brief Check if the input is allow to be parse
 * 0 if the input is accepted
 * 1 if the input is empty
 * 2 if the input has a syntax error
 * @param input 
 * @return int 
 */
int	syntax_errors(char *input)
{
	size_t	i;
	char	*error_token;
	char	*start_ptr;

	i = 0;
	error_token = NULL;
	start_ptr = input;
	while (*input)
	{
		printf("input : '%s'\n", input);
		if (is_syntax_char(input))
		{
			if (check_syntax_at(&input, &error_token, start_ptr) < 1)
			{
				ft_write_error(NULL, NULL, error_token);
				free(error_token);
				return (2); // write return message here with error_token
			}
		}
		input++;
	}
	return (i == 0);
}

/**
 * @brief Check if a parenthesis is closed or is empty.
 * 
 * @param input 
 * @return true 
 * @return false 
 */
bool	has_parenthesis_not_closed(const char *input)
{
	size_t	depth;
	int		i;
	int		j;

	depth = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '(')
		{
			j = 1;
			while (is_wspace(input[i + j]))
				j++;
			if (input[i + j] == ')')
				return (true);
		}
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		else if (input[i] == '(')
			depth++;
		else if (input[i] == ')')
			depth--;
		i++;
	}
	return (depth != 0);
}

bool	has_argument_left(const char *start_input, char *op_ptr, 
						char **error_token)
{
	char	*tmp;

	tmp = op_ptr;
	while (tmp != start_input)
	{
		tmp--;
		if (*tmp == '(')
			return (*error_token = get_error_token(op_ptr), false);
		if (tmp != start_input && is_operator(tmp - 1))
			return (*error_token = get_error_token(op_ptr), false);
		else if (!is_wspace(*tmp))
			return (true);
	}
	*error_token = get_error_token(tmp);
	return (false);
}

/**
 * @brief Get the right argument of the operator.
 * Set error_token if the argument is not valid.
 * 
 * @param op_ptr 
 * @param error_token 
 * @return true 
 * @return false 
 */
bool	has_argument_right(char *op_ptr, char **error_token)
{
	char	*op_cpy;

	op_cpy = op_ptr;
	if (*(op_ptr + 1) == *op_ptr)
		op_ptr++;
	op_ptr++;
	while (*op_ptr)
	{
		if (*op_ptr == ')')
			return (*error_token = get_error_token(op_cpy), false);
		else if (!is_wspace(*op_ptr))
			return (true);
		op_ptr++;
	}
	*error_token = get_error_token(op_cpy);
	return (false);
}
