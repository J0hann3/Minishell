/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 00:59:40 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 18:21:02 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_operator(const char *c)
{
	return (is_and_or(c) || *c == '|');
}

bool	is_single_meta(const char *c)
{
	return (
		(*c == '|' && *(c + 1) != '|')
		|| (*c == '>' && *(c + 1) != '>')
		|| (*c == '<' && *(c + 1) != '<'));
}

bool	is_double_meta(const char *c)
{
	return (
		(*c == '|' && *(c + 1) == '|')
		|| (*c == '&' && *(c + 1) == '&')
		|| (*c == '>' && *(c + 1) == '>')
		|| (*c == '<' && *(c + 1) == '<'));
}

bool	is_wspace(int c)
{
	return (c == ' ' || c == '\t');
}

bool	is_and_or(const char *input)
{
	return ((*input == '&' && *(input + 1) == '&')
		|| (*input == '|' && *(input + 1) == '|'));
}
