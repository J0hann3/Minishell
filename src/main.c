/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/10 19:02:28 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int parse(char *input)
{
	// check " '
	// check >
	// check <
	// check 
	// split on white spaces
	// 

	return (1);
}

int main()
{
	char *input;

	while (1)
	{
		input = readline("minishell: ");
		parse(input);
	}
	return (0);
}
