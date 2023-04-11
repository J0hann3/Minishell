/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 22:13:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/11 21:58:07 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tree(t_ast *tree)
{
	if (tree == NULL)
		return ;
	if (tree->left == NULL || tree->right == NULL)
	{
		// free(tree->command);
		free(tree);
		return ;
	}
	free_tree(tree->left);
	free_tree(tree->right);
}
