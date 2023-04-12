/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:23:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/11 19:24:51 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

int	get_height(t_ast *root)
{
	int	leftHeight;
	int	rightHeight;

	if (!root)
		return (0);
	else
	{
		leftHeight = get_height(root->left);
		rightHeight = get_height(root->right);
		if (leftHeight > rightHeight)
			return (leftHeight + 1);
		else
			return (rightHeight + 1);
	}
}

const char *meta_to_char(enum e_meta_character meta)
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