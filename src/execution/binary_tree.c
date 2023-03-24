/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/24 16:26:41 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

void	explore_tree(t_node *tree, t_env_info *env, enum e_meta_character operand, int status)
{
	int	pid;
	int	stat;

	if (tree == NULL)
		return;
	if (tree->left == NULL && tree->right == NULL && tree->command != NULL)
	{
		if (operand == e_and)
		{
			if (status == e_true)
				exec(tree->command, env);
			else
				return ; 
			
		}
		else if (operand == e_or)
		{
			if (status == e_false)
				exec(tree->command, env);
			else
				return ;
		}
		else if (operand == e_pipe)
		{
			pid = ft_pipe(env);			//ATTENTION need to close fd after use
			if (pid == 0)
			{
				return (exec(tree->command, env));
			}
			else
				waitpid(pid, &stat, 0);
				if (WIFEXITED(stat))
					env->error = WEXITSTATUS(stat);
				else 
					env->error = 1;
				explore_tree(tree->right, env, e_pipe, env->error);
		}
	}
		
	if (tree->meta != e_empty)
		//changes operand to call function
	explore_tree(tree->left, env);
	explore_tree(tree->right, env);
}