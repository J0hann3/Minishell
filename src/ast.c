/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:08:36 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/22 01:23:07 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

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


t_ast	*create_tree(char *input, t_ast *parent)
{
	t_ast	*node;

	//node create
	if (!is_sub_tree(input)) // if has not operator next to
	{
		return (create_leaf(input));
	}
	node->left = create_tree(input, node);
	node->meta = get_meta();
	node->right = create_tree(input + x, node);
	return (node);
}

// si parentheses : create tree
// sinon create leaf

// a -> create leaf return
// && prend a comme left puis creer leaf b, pui return
// create leaf b








//t_ast	*create_tree(char *input, t_ast *parent)
//{
//	t_ast	*node;
//	char	*cur_command;

//	cur_command = NULL;

//	while (*input)
//	{
//		if (*input == '(')
//		{
//			//create_tree(input, node);
//		}
//		else if (is_double_meta(input) || is_single_meta(input))
//		{
//			node = ast_new_node(NULL);
//			if (!node)
//				return (NULL); // secure free
//			node->meta = get_meta_type(input);
//			node->size = get_meta_size(node->meta);
//			node->parent = NULL;

//		}
//		else if (!is_wspace(*(input)))
//		{
//			node = ast_new_node(input);
//			if (!node)
//				return (NULL); // secure free
//			node->size = get_command_size(input);
//			node->parent = parent;
//			node->meta = e_empty;
//			return (node);
//		}
//	}
//	return (node);
//}

// si parentheses : create tree
// sinon create leaf

// a -> create leaf return
// && prend a comme left puis creer leaf b, pui return
// create leaf b