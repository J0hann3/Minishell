/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_rec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 03:36:18 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 04:09:08 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast	*on_left_branch(char **input, t_ast *child, bool *has_error)
{
	t_ast	*left;

	while (**input == ' ')
		(*input)++;
	if (child)
		left = child;
	else if (**input == '(')
	{
		(*input)++;
		left = create_sub_tree(input, NULL, has_error);
		if (**input == ')')
			(*input)++;
	}
	else
	{
		left = create_leaf(*input);
		if (!left)
			return (*has_error = 1, NULL);
		(*input) += left->size;
	}
	return (left);
}

static void	on_right_branch(char **input, t_ast *left, bool *has_error)
{
	while (**input == ' ')
		(*input)++;
	if (**input == '(')
	{
		(*input)++;
		left->parent->right = create_sub_tree(input, NULL, has_error);
		left->parent->right->parent = left->parent;
		if (**input == ')')
			(*input)++;
	}
	else
	{
		left->parent->right = create_leaf(*input);
		if (!left->parent->right)
			return ;
		*input += left->parent->right->size;
	}
}

t_ast	*create_sub_tree(char **input, t_ast *child, bool *has_error)
{
	t_ast	*left;

	left = on_left_branch(input, child, has_error);
	while (**input == ' ')
		(*input)++;
	if (!**input || !is_operator(*input))
		return (left);
	left->parent = create_op_node(*input);
	left->parent->left = left;
	*input += left->parent->size;
	on_right_branch(input, left, has_error);
	left->parent->right->parent = left->parent;
	while (**input == ' ')
		(*input)++;
	if (is_operator(*input))
		return (create_sub_tree(input, left->parent, has_error));
	return (left->parent);
}
