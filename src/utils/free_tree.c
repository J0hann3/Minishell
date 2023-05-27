/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 22:13:12 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/27 13:10:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tree(t_ast **tree)
{
	t_ast	*tmp;

	if (tree == NULL || *tree == NULL)
		return ;
	tmp = *tree;
	free_tree(&tmp->left);
	free_tree(&tmp->right);
	if ((*tree)->fd_heredocs >= 0)
		close((*tree)->fd_heredocs);
	free(*tree);
	tree = NULL;
}

char	*find_most_left_command(t_ast *tree)
{
	if (tree == NULL)
		return (NULL);
	if (tree->left == NULL)
		return (tree->command);
	return (find_most_left_command(tree->left));
}
