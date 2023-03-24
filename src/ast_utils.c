/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:23:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/22 17:56:53 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

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
