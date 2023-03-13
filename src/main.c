/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/13 02:42:24 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	parse(char *input)
{
	size_t	i;

	if (equals(input, "exit") == 1)
		free(input), exit(EXIT_SUCCESS);

	// trim/split on white spaces
	// 
	// create tree

	if (quotes_not_closed((const char *)input))
		return (2);
	i = 0;
	while (input[i])
	{
		if (is_meta_character(input[i]))
		{
			if (input[i] == '&' && input[i + 1] != '&')
			{
				i++;
				continue ;
			}
			if (input[i] == '<' || input[i] == '>')
			{
				if (!has_argument_right(input + i))
					return (2);
			}
			else if(!has_argument_left(input, (input + i))
				|| !has_argument_right(input + i))
				return (2);
		}
		i++;
	}
	return (1);
}

int main(int argc, char *argv[], char *env[])
{
	char	*input;

	(void)argc;
	(void)argv;
	(void)env;

	input = (char *)1;
	while (input != NULL)
	{
		input = readline("minishell$>");
		printf("out : %d\n", parse(input));
		free(input);
	}
	return (0);
}
