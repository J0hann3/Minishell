/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/16 18:32:47 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

bool	has_error_on_parenthesis(const char *input)
{
	size_t	depth;
	int		i;

	depth = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input) + 1;
		if (input[i] == '(')
			depth++;
		else if (input[i] == ')')
			depth--;
		i++;
	}
	return (depth != 0);
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
	else if (input[i] == '(' || input[i] == ')')
	{
		// TODO: check if parenthesis are not empty inside
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
		return (3);
	else
		printf("ok\n");
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		if (is_meta_character(input[i]) && has_error_for_meta(input, i))
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
