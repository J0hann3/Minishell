/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/17 05:41:41 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

bool	has_error_op_par_right(const char *input)
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


bool	has_error_on_operators_and_parenthesis(const char *input)
{
	bool	has_op_left;
	size_t	i;

	while (is_wspace(*input))
		input++;
	has_op_left = false;
	if (*input == '(')
		has_op_left = true;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		else if (input[i] == '(' && !has_op_left)
			return (true);
		else if (input[i] == ')' && has_error_op_par_right(input + i))
			return (true);
		else if (is_and_or(input + i) || input[i] == '|' || input[i] == '(')
			has_op_left = ++i;
		else if (!is_wspace(input[i]))
			has_op_left = false;
		i++;
	}
	return (false);
}

bool	has_error_on_parenthesis(const char *input)
{
	size_t	depth;
	int		i;

	depth = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '(' && input[i + 1] == ')')
			return (true);
		else if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		else if (input[i] == '(')
			depth++;
		else if (input[i] == ')')
			depth--;
		i++;
	}
	return (depth != 0);
}

void	replace_parenthesis(char *string)
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

bool	is_useful_parenthesis(const char *string)
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
			i += skip_quotes(string);
		else if (string[i] == '(')
		{
			if (!is_useful_parenthesis(string + i))
				replace_parenthesis(string + i);
		}
		i++;
	}
}

bool	has_error_for_meta(char *input, size_t i)
{
	if (input[i] == '&' && input[i + 1] != '&')
		return (0);
	else if (input[i] == '<' || input[i] == '>')
	{
		if (!has_argument_right(input + i))
			return (1);
	}
	else if (input[i] == '|' || input[i] == '&')
	{
		if (!has_argument_left(input, (input + i))
			|| !has_argument_right(input + i))
			return (1);
	}
	return (0);
}

int	syntax_errors(char *input)
{
	size_t	i;

	if (eq(input, "exit") == 1) // temp
		free(input), exit(EXIT_SUCCESS);

	if (quotes_not_closed((const char *)input))
		return (2);
	if (has_error_on_parenthesis(input))
		return (2);
	remove_useless_parenthesis(&input);
	if (has_error_on_operators_and_parenthesis(input))
		return (2);
	printf("canon : '%s'\n", input);
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		if ((is_meta_character(input[i])) && has_error_for_meta(input, i))
			return (2);
		i++;
	}
	return (0);
}

int main(int argc, char *argv[], char *env[])
{
	char	*input;
	int		ret_err;

	(void)argc;
	(void)argv;
	(void)env;
	input = (char *)1;
	ret_err = 0;
	while (input != NULL)
	{
		input = readline("minishell$>");
		if (!input)
			break ;
		add_history(input);
		ret_err = syntax_errors(input);
		printf("err : %d\n", ret_err);
		parse_args(input);
		free(input);
	}
	rl_clear_history();
	return (ret_err);
}
