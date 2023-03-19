/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/19 18:02:45 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

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

void	remove_multiple_wspaces(char *input)
{
	char	*dest;
	size_t	space_count;

	space_count = 1;
	dest = input;
	while (*input)
	{
		if (*input == ' ' || *input == '\t')
			space_count++;
		else
			space_count = 0;
		if (space_count <= 1)
		{
			if (*input == '\t')
				*dest++ = ' ';
			else
				*dest++ = *input;
		}
		(input)++;
	}
	if (space_count >= 1)
		*(dest - 1) = 0;
	else
		*dest = 0;
}

int	syntax_errors(char *input)
{
	size_t	i;

	if (eq(input, "exit") == 1) // temp
		free(input), exit(EXIT_SUCCESS);
	if (quotes_not_closed(input))
		return (2);
	if (has_parenthesis_not_closed(input))
		return (2);
	remove_useless_parenthesis(&input);
	if (has_error_on_operators_and_parenthesis(input))
		return (2);
	remove_multiple_wspaces(input);
	remove_multiple_wspaces(input);
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
