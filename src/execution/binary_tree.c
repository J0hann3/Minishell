/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:59:00 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/23 18:08:09 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	explore_tree(t_node *tree, t_env_info *env)
{
	if (tree == NULL)
		return ;
	if (tree->left == NULL && tree->right == NULL)
		//command to exec
	explore_tree(tree->left);
	explore_tree(tree->right);
}