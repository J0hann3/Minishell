/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:23:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 03:42:02 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_node(const char *command)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->command = (char *)command;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->meta = 0;
	node->size = 0;
	return (node);
}

enum e_meta_character	get_meta(char *input)
{
	if (is_operator(input))
	{
		if (is_and_or(input))
		{
			if (*input == '|')
				return (e_or);
			return (e_and);
		}
		return (e_pipe);
	}
	return (e_empty);
}

const char	*meta_to_char(enum e_meta_character meta)
{
	if (meta == e_pipe)
		return ("|");
	if (meta == e_or)
		return ("||");
	if (meta == e_and)
		return ("&&");
	else
		return ("");
}

size_t	get_command_size(const char *input)
{
	const char	*start;

	start = input;
	while (*input)
	{
		if (is_double_meta(input) || is_single_meta(input))
			return (start - input);
		input++;
	}
	return (start - input);
}
