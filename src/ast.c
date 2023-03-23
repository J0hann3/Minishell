/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:08:36 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/23 02:22:31 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

static enum e_meta_character	get_meta(char *input)
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

t_ast	*create_leaf(const char *input)
{
	t_ast		*node;
	const char	*start;

	start = input;
	node = create_node(input);
	if (!node)
		return (NULL);
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
			input += skip_parenthesis(input);
		else if (*input == ')' || is_operator(input))
			break ;
		input++;
	}
	node->size = input - start;
	return (node);
}

t_ast	*create_op_node(char *input)
{
	t_ast	*node;

	while (*input == ' ')
		input++;
	node = create_node(NULL);
	if (!node)
		return (NULL);
	if (is_operator(input))
	{
		node->meta = get_meta(input);
		if (is_and_or(input))
			node->size = 2;
		else
			node->size = 1;
	}
	return (node);
}

t_ast	*create_sub_tree(char **input, t_ast *child)
{
	t_ast	*left;

	while (**input == ' ')
		(*input)++;
	if (child)
		left = child;
	else if (**input == '(')
	{
		(*input)++;
		left = create_sub_tree(input, NULL);
		(*input) += left->size;
	}
	else
	{
		left = create_leaf(*input);
		(*input) += left->size;
	}
	while (**input == ' ')
		input++;
	if (!**input || !is_operator(*input))
		return (left);
	left->parent = create_op_node(*input);
	left->parent->left = left;
	*input += left->parent->size;
	while (**input == ' ')
		(*input)++;
	if (**input == '(')
	{
		(*input)++;
		left->parent->right = create_sub_tree(input, NULL);
		if (**input == ')')
		{
			(*input)++;
			return (left->parent);
		}
	}
	else
	{
		left->parent->right = create_leaf(*input);
		*input += left->parent->right->size;
		if (**input == ')')
		{
			(*input)++;
			return (left->parent);
		}
	}
	while (**input == ' ')
		(*input)++;
	if (is_operator(*input))
		return (create_sub_tree(input, left->parent));
	return (left->parent);
}

void	print_tree(t_ast *tree, int depth)
{
	if (!tree)
		return ;
	depth++;
	if (tree->left)
	{
		print_tree(tree->left, depth);
	}
	if (tree->command)
	{
		write(1, tree->command, tree->size);
		printf("(%d)\n", depth);
	}
	else
		printf("%d (%d)\n", tree->meta, depth);
	if (tree->right)
	{
		print_tree(tree->right, depth);
	}
}

t_ast	*create_tree(char *input)
{
	t_ast	*root;

	root = create_sub_tree(&input, NULL);
	print_tree(root, 0);
	return (NULL);
}
