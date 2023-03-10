/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/10 21:18:22 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	check_quotes(const char *input)
{
	int	in_single;
	int	in_double;

	in_single = -1;
	in_double = -1;
	while (*input)
	{
		if (*input == '\"' && in_single == -1)
			in_double *= -1;
		if (*input == '\'' && in_double == -1)
			in_single *= -1;
		input++;
	}
	return (in_double == 1 || in_single == 1);
}
