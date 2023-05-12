/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/13 00:14:12 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


//TODO : modifier les error_token et les enlever


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
 * closing quote, and write error if closing quote does not exist
 * 
 * @param input 
 * @return true 
 * @return false 
 */
bool	is_quote_closed(char **input)
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
	ft_write_error(NULL, NULL, (char *)error_quote);
	return (false);
}

/**
 * @brief Check if the redirection has a valid argument
 * Forward input at the last charcater of the argument.
 * If no valid argument, write errorof the redirection.
 * This function call heredoc open if encounterd.
 * This function can call : 
 * 		is_quote_closed
 * 		do_here_docs
 * @param input 
 * @return true 
 * @return false 
 */
bool	is_redirection_ok(char **input, t_env_info *env)
{
	char	*start;
	bool	is_here_doc;
	int		fd_heredoc;
	int		error;

	is_here_doc = false;
	if (**input != *(*input + 1) && is_redirection(*(*input + 1)))
		return (ft_write_error(NULL, NULL, get_error_token(*input + 1)), false);
	if (**input == '<' && *(*input + 1) == '<')
		is_here_doc = true;						//heredocs is created if command = ><test
	(*input)++;
	if (**input == '>' || **input == '<')
		(*input)++;
	while (is_wspace((**input)))
		(*input)++;
	start = *input;
	while (**input && !is_wspace(**input) && !is_operator((*input))
			&& !is_parenthesis(**input) && !is_redirection(**input))
	{
		if ((**input == '\'' || **input == '"')
			&& !is_quote_closed(input))
				return (false);
		(*input)++;
	}
	if ((*input) == start)
		return (ft_write_error(NULL, NULL, get_error_token(*input)), false);
	(*input)--;
	if (is_here_doc)
	{
		if (env->fds_heredocs[env->len_heredocs] != -1)
			close(env->fds_heredocs[env->len_heredocs]);
		error = do_here_docs(start, env, &fd_heredoc);
		if (error == 130)
		{
			if (env->fds_heredocs[0] >= 0)
				close(env->fds_heredocs[0]);
			return (close(fd_heredoc), env->fds_heredocs[0] = -2, false);
		}
		else if (error != 0)
		{
			if (env->fds_heredocs[0] >= 0)
				close(env->fds_heredocs[0]);
			return (close(fd_heredoc), env->fds_heredocs[0] = -1, false);
		}
		env->fds_heredocs[env->len_heredocs] = fd_heredoc;
	}
	return (true);
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

static const char *get_par_error(char *par_start)
{
	size_t	i;
	
	par_start++;
	i = 0;
	while (par_start[i] && is_wspace(par_start[i]))
		i++;
	while (par_start[i] && !is_operator(par_start + i) && !is_redirection(par_start[i])
			&& !is_wspace(par_start[i]) && !is_parenthesis(par_start[i]))
		i++;
	par_start[i] = 0;
	return (par_start);
}

bool	is_parenthesis_left_ok(char *par_start, char *start_ptr)
{
	char *tmp;
	char *parenthesis;

	parenthesis = par_start;
	if (par_start == start_ptr)
		return (true);
	par_start--;
	while (par_start != start_ptr && is_wspace(*par_start))
		par_start--;
	if (*par_start == '(')
		return (true);
	tmp = par_start;
	if (*par_start == '|')
		while (par_start != start_ptr && *par_start == '|')
			par_start--;
	else
		while (par_start != start_ptr && *par_start == '&')
			par_start--;
	if (tmp - par_start == 2 && *(par_start + 1) == '&')
		return (true);
	if (tmp - par_start == 1 && *(par_start + 1) == '|')
		return (true);
	if (tmp - par_start == 2 && *(par_start + 1) == '|')
		return (true);
	tmp = ft_strjoin3("syntax error near unexpected token `", get_par_error(parenthesis), "'");
	return (ft_write_error(NULL, NULL, tmp), free(tmp), false);
}

bool	is_parenthesis_right_ok(char *str)
{
	size_t	i;
	int		tmp;
	char	*error;

	i = 1;
	while (str[i] && is_wspace(str[i]))
		i++;
	if (str[i] == '\0' || str[i] == ')')
		return (true);
	tmp = i;
	if (str[i] == '|')
		while (str[i] && str[i] == '|')
			i++;
	else
		while (str[i] && str[i] == '&')
			i++;
	if (i - tmp == 2 && str[i - 1] == '&')
		return (true);
	if (i - tmp == 1 && str[i - 1] == '|')
		return (true);
	if (i - tmp == 2 && str[i - 1] == '|')
		return (true);
	error = ft_strjoin3("syntax error near unexpected token `", get_par_error(str), "'");
	return (ft_write_error(NULL, NULL, error), free(error), false);
}

/**
 * @brief Checks if the given parenthesis is closed, forward input to the
 * closing parenthesis, and write error if closing parenthesis does not exist
 * Will check if the parenthesis has a left 
 * 
 * @param input 
 * @return true 
 * @return false 
 */
bool	has_closing_parenthesis(char **input, t_env_info *env, char *start_ptr)
{
	char	*par_start;
	const char *error_parenth = "unexpected EOF while looking for matching \
`('\nminishell: syntax error: unexpected end of file";

	par_start = (*input);
	(*input)++;
	while (is_wspace((**input)))
		(*input)++;
	if (**input == ')')
	{
		ft_write_error(NULL, NULL, "syntax error near unexpected token `)'"); //write error
		return (false);
	}
	if (!is_parenthesis_left_ok(par_start, start_ptr))
		return (false);
	while (**input)
	{
		if (**input == ')')
			return (is_parenthesis_right_ok(*input));
		else if (**input == '(' && !has_closing_parenthesis(input, env, start_ptr))
			return (false);
		else if ((**input == '\'' || **input == '"')
				&& !is_quote_closed(input))
			return (false);
		else if ((**input == '<' || **input == '>')
				&& !is_redirection_ok(input, env))
			return (false);
		else if (is_operator(*input))
		{
			if (!is_operator_ok(input, par_start))
				return (false);
			else
			{
				env->len_heredocs++;
				env->fds_heredocs = ft_realloc(env->fds_heredocs, env->len_heredocs * sizeof(int), (env->len_heredocs + 1) * sizeof(int));
				if (!env->fds_heredocs)
					return (mem_exh("heredocs"), false);
				env->fds_heredocs[env->len_heredocs] = -1;
			}
		}
		(*input)++;
	}
	ft_write_error(NULL, NULL, (char *)error_parenth);
	return (false);
}

/**
 * @brief
 * return -1 on unexpected error
 * return 0 if syntax error and the write the error at the syntax error chars
 * return 1 if the syntax is ok
 * 
 * @param input 
 * @return int 
 */
int	check_syntax_at(char **input, char *start_ptr, t_env_info *env)
{
	if (**input == '\'' || **input == '"')
		return (is_quote_closed(input));
	if (**input == '<' || **input == '>')
		return (is_redirection_ok(input, env));
	if (is_operator(*input))
		return (is_operator_ok(input, start_ptr) * 2);
	if (**input == '(')
		return (has_closing_parenthesis(input, env, start_ptr));
	if (**input == ')')
		return (ft_write_error(NULL, NULL, "syntax error near unexpected token `)'"), false);
	return (-1);
}


/**
 * @brief Check if the input is allow to be parse
 * 0 if the input is accepted
 * 1 if the input is empty
 * 2 if the input has a syntax error
 * 
 * @param input 
 * @return int 
 */
int	syntax_errors(char *input, t_env_info *env)
{
	char	*start_ptr;
	int		ret_val;

	env->len_heredocs = 0;
	env->fds_heredocs = malloc(1 * sizeof(int));
	if (!env->fds_heredocs)
		return (mem_exh("heredocs"), 2);
	env->fds_heredocs[env->len_heredocs] = -1;
	remove_multiple_wspaces(input);
	start_ptr = input;
	while (*input)
	{
		if (is_syntax_char(input))
		{
			ret_val = check_syntax_at(&input, start_ptr, env);
			if (ret_val < 1) //error syntax
			{
				if (env->fds_heredocs[0] == -2)
					return (130);
				return (2); // write return message here with error_token
			}
			else if (ret_val == 2) //new operator for heredocs
			{
				env->len_heredocs++;
				env->fds_heredocs = ft_realloc(env->fds_heredocs, env->len_heredocs * sizeof(int), (env->len_heredocs + 1) * sizeof(int));
				if (!env->fds_heredocs)
					return (mem_exh("heredocs"), 2);
				env->fds_heredocs[env->len_heredocs] = -1;
			}
		}
		input++;
	}
	return (start_ptr == input);
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

bool	has_argument_left(const char *start_input, char *op_ptr)
{
	char	*tmp;

	tmp = op_ptr;
	while (tmp != start_input)
	{
		tmp--;
		if (*tmp == '(')
			return (ft_write_error(NULL, NULL, get_error_token(op_ptr)), false);
		if (*tmp == '|' || (tmp != start_input && is_and_or(tmp - 1)))
			return (ft_write_error(NULL, NULL, get_error_token(op_ptr)), false);
		else if (!is_wspace(*tmp))
			return (true);
	}
	ft_write_error(NULL, NULL, get_error_token(tmp));
	return (false);
}

/**
 * @brief Get the right argument of the operator.
 * Write error if the argument is not valid.
 * 
 * @param op_ptr 
 * @return true 
 * @return false 
 */
bool	has_argument_right(char *op_ptr)
{
	char	*op_cpy;

	op_cpy = op_ptr;
	if (*(op_ptr + 1) == *op_ptr)
		op_ptr++;
	op_ptr++;
	while (*op_ptr)
	{
		if (*op_ptr == ')')
			return (ft_write_error(NULL, NULL, get_error_token(op_cpy)), false);
		if (is_operator(op_ptr))
			return (ft_write_error(NULL, NULL, get_error_token(op_cpy)), false);
		else if (!is_wspace(*op_ptr))
			return (true);
		op_ptr++;
	}
	ft_write_error(NULL, NULL, get_error_token(op_cpy));
	return (false);
}
