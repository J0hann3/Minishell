/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/24 15:16:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	explore_tree(t_node *tree, t_env_info *env, enum e_meta_character operand, )
{
	int	last_error;

	if (tree == NULL)
		return;
	last_error = env->error;
	env->error = 0;
	if (tree->left == NULL && tree->right == NULL)
		//execute command
	explore_tree(tree->left, env);
	explore_tree(tree->right, env);
}