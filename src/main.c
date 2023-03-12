/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/12 20:24:47 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	parse(char *input)
{
	if (equals(input, "exit") == 1)
		free(input), exit(EXIT_SUCCESS);

	// check " '
	// check >
	// check <
	// check double single meta
	// trim/split on white spaces
	// 
	// create tree

	if (quotes_not_closed((const char *)input))
		return (2);
	//printf("%d\n", quotes_not_closed((const char *)input));
	if (has_error_syntax_pipe(input))
		return (2);




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
