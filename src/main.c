/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/10 21:23:30 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int parse(char *input)
{
	// check " '
	// check >
	// check <
	// check double single meta
	// trim/split on white spaces
	// 
	// create tree


	printf("%d\n", check_quotes((const char *)input));





	return (1);
}

int	exec(int argc, char *argv[], char *env[])
{
	char *input;

	while (1)
	{
		input = readline("minishell: ");
		parse(input);
	}
	return (0);
}

int main(int argc, char *argv[], char *env[])
{

}
