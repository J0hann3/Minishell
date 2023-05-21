/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:08:36 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 04:09:50 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static int	get_height(t_ast *root)
// {
// 	int	left_height;
// 	int	right_height;

// 	if (!root)
// 		return (0);
// 	else
// 	{
// 		left_height = get_height(root->left);
// 		right_height = get_height(root->right);
// 		if (left_height > right_height)
// 			return (left_height + 1);
// 		else
// 			return (right_height + 1);
// 	}
// }

// static void	print_tree(t_ast *tree, int depth, int max_depth)
// {
// 	int	i;

// 	if (!tree)
// 		return ;
// 	i = 0;
// 	depth++;
// 	if (tree->left)
// 		print_tree(tree->left, depth, max_depth);
// 	while (i++ < max_depth - (depth))
// 		write(1, "\t\t", 2);
// 	if (tree->command)
// 	{
// 		write(1, "[", 1);
// 		write(1, tree->command, tree->size);
// 		write(1, "]\n", 2);
// 	}
// 	else
// 	{
// 		write(1, "[", 1);
// 		write(1, meta_to_char(tree->meta), tree->size);
// 		write(1, "]\n", 2);
// 	}
// 	if (tree->right)
// 		print_tree(tree->right, depth, max_depth);
// }

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

void	add_heredocs(t_ast *tree, int *fd_heredocs, int *fd_size, int size_max)
{
	if (!tree)
		return ;
	if (tree->left)
		add_heredocs(tree->left, fd_heredocs, fd_size, size_max);
	if (tree->command != NULL)
	{
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
	t_ast	*tree;
	int		i;
	bool	has_error;

	i = 0;
	has_error = false;
	tree = create_sub_tree(&input, NULL, &has_error);
	add_heredocs(tree, fd_heredocs, &i, len_fd);
	if (has_error)
		return (free_tree(&tree), mem_exh("ast creation"), NULL);
	return (tree);
}
