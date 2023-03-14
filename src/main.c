/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/14 05:36:32 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"
#include "../includes/get_next_line.h"

int	has_error_for_meta(char *input, size_t i)
{
	if (input[i] == '&' && input[i + 1] != '&')
		return (0);
	else if (input[i] == '<' || input[i] == '>')
	{
		if (!has_argument_right(input + i))
			return (1);
	}
	else if(!has_argument_left(input, (input + i))
		|| !has_argument_right(input + i))
		return (1);
	return (0);
}

int	syntax_errors(char *input)
{
	size_t	i;
	int		in_single;
	int		in_double;

	if (equals(input, "exit") == 1)
		free(input), exit(EXIT_SUCCESS);
	if (quotes_not_closed((const char *)input))
		return (2);
	i = 0;
	in_double = -1;
	in_single = -1;
	while (input[i])
	{
		if (input[i] == '\"' && in_single == -1)
			in_double *= -1;
		if (input[i] == '\'' && in_double == -1)
			in_single *= -1;
		if ((in_double == 1 || in_single == 1) && ++i)
			continue ;
		if (is_meta_character(input[i]) && has_error_for_meta(input, i))
			return (2);
		i++;
	}
	return (1);
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
		input = readline("Minishell$>");
		if (!input)
			break ;
		ret_err = syntax_errors(input);
		free(input);
	}
	return (ret_err);
}
