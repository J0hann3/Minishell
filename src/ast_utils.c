/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:23:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/15 03:37:47 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

t_ast	*ast_new_node(char *command)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->command = command;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->meta = 0;
	node->size = 0;
	return (node);
}
