/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_parenthesis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 16:29:47 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/01 21:31:02 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	replace_parenthesis(char *string)
{
	size_t	i;

	*string = ' ';
	i = 1;
	while (string[i])
	{
		if (string[i] == '(')
			i += skip_parenthesis(string + i);
		else if (string[i] == ')')
		{
			string[i] = ' ';
			return ;
		}
		i++;
	}
}

static bool	is_useful_parenthesis(const char *string)
{
	bool	has_op;
	size_t	i;

	has_op = false;
	i = 1;
	while (string[i])
	{
		if (string[i] == '\"' || string[i] == '\'')
			i += skip_quotes(string + i);
		else if (string[i] == '(')
			i += skip_parenthesis(string + i);
		else if (string[i] == ')')
			break ;
		else if (is_and_or(string + i))
		{
			has_op = true;
			break ;
		}
		i++;
	}
	return (has_op);
}

void	remove_useless_parenthesis(char **input)
{
	char	*string;
	size_t	i;

	string = *input;
	i = 0;
	while (string[i])
	{
		if (string[i] == '\"' || string[i] == '\'')
			i += skip_quotes(string + i);
		else if (string[i] == '(')
		{
			if (!is_useful_parenthesis(string + i))
				replace_parenthesis(string + i);
		}
		i++;
	}
}

static bool	has_error_op_par_right(const char *input)
{
	size_t	i;
	bool	has_op_right;

	i = 0;
	has_op_right = false;
	while (input[i])
	{
		if (is_and_or(input + i) || input[i] == '|')
		{
			has_op_right = true;
			break ;
		}
		else if (!is_wspace(input[i]) && input[i] != ')')
			return (true);
		i++;
	}
	if (!has_op_right && input[i])
		return (true);
	return (false);
}

/**
 * @brief Warning : can have non closing parenthesis at this point
 * 
 * @param input 
 * @return true 
 * @return false 
 */
bool	has_error_on_operators_and_parenthesis(const char *input)
{
	bool	has_op_left;
	size_t	i;

	while (is_wspace(*input))
		input++;
	has_op_left = (*input == '(');
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		else if (input[i] == '(' && !has_op_left)
		{
			return (true); //error on token before, can't be empty
		}
		else if (input[i] == ')' && has_error_op_par_right(input + i))
		{
			return (true); //error on token after '(' (not newline)
		}
		else if (is_and_or(input + i))
			has_op_left = ++i;
		else if (is_single_meta(input + i))
			has_op_left = 1;
		else if (!is_wspace(input[i]) && input[i] != '(')
			has_op_left = false;
		i++;
	}
	return (false); // no error
}
