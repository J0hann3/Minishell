/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:08:36 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/05 19:48:13 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

enum e_meta_character	get_meta(char *input);
int						get_height(t_ast *root);
const char				*meta_to_char(enum e_meta_character meta);

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
			input += skip_quotes(input);
		else if (*input == ')' || is_operator(input))
			break ;
		input++;
	}
	node->size = input - start;
	if (input != start && *(input - 1) == ' ')
		node->size--;
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
		if (**input == ')')
			(*input)++;
	}
	else
	{
		left = create_leaf(*input);
		(*input) += left->size;
	}
	while (**input == ' ')
		(*input)++;
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
		left->parent->right->parent = left->parent;
		if (**input == ')')
			(*input)++;
	}
	else
	{
		left->parent->right = create_leaf(*input);
		*input += left->parent->right->size;
	}
	left->parent->right->parent = left->parent;
	while (**input == ' ')
		(*input)++;
	if (is_operator(*input))
		return (create_sub_tree(input, left->parent));
	return (left->parent);
}

void	print_tree(t_ast *tree, int depth, int max_depth)
{
	int	i;

	if (!tree)
		return ;
	i = 0;
	depth++;
	if (tree->left)
		print_tree(tree->left, depth, max_depth);
	while(i++ < max_depth - (depth))
		write(1, "\t\t", 2);
	if (tree->command)
	{
		write(1, "[", 1);
		write(1, tree->command, tree->size);
		write(1, "]\n", 2);
	}
	else
	{
		write(1, "[", 1);
		write(1, meta_to_char(tree->meta), tree->size);
		write(1, "]\n", 2);
	}
	if (tree->right)
		print_tree(tree->right, depth, max_depth);
}

void	add_heredocs(t_ast *tree, int *fd_heredocs, int *fd_size, int size_max)
{
	if (!tree)
		return ;
	if (tree->left)
		add_heredocs(tree->left, fd_heredocs, fd_size, size_max);
	if (tree->command != NULL)
	{
		if (*fd_size > size_max)
			return ((void)printf("ERROR fd heredocs\n"));
		tree->fd_heredocs = fd_heredocs[*fd_size];
		fd_heredocs[*fd_size] = -1;
		(*fd_size)++;
	}
	else
		tree->fd_heredocs = -2;
	if (tree->right)
		add_heredocs(tree->right, fd_heredocs, fd_size, size_max);
}


t_ast	*create_tree(char *input, int *fd_heredocs, int len_fd)
{
	t_ast	*root;
	int		i;

	i = 0;
	root = create_sub_tree(&input, NULL);
	add_heredocs(root, fd_heredocs, &i, len_fd);
	// print_tree(root, 0, get_height(root));
	return (root);
}
