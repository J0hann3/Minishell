/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/24 17:39:42 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

void	explore_tree(t_node *tree, t_env_info *env, enum e_meta_character operand, int status)
{
	char	**arg;
	int	pid;
	int	stat;

	if (tree == NULL)
		return;
	if (tree->left == NULL && tree->right == NULL && tree->command != NULL)
	{
		if (operand == e_and)
		{
			if (status == e_true)
			{
				arg = parsing2(tree->command);
				exec(arg, env);
			}
			else
				return ; 
			
		}
		else if (operand == e_or)
		{
			if (status == e_false)
			{
				arg = parsing2(tree->command);
				exec(arg, env);
			}
			else
				return ;
		}
		else if (operand == e_pipe)
		{
			pid = ft_pipe(env);			//ATTENTION need to close fd after use
			if (pid == 0)
			{
				arg = parsing2(tree->command);
				exec(arg, env);
			}
			else
				waitpid(pid, &stat, 0);
				// if (WIFEXITED(stat))
				// 	env->error = WEXITSTATUS(stat);
				// else 
				// 	env->error = 1;
				explore_tree(tree->right, env, e_pipe_right, 0);
		}
		else if (operand == e_pipe_right)
		{
			arg = parsing2(tree->command);
			exec(arg, env);
		}
	}
		
	if (tree->meta != e_empty)
		operand = tree->meta;
	explore_tree(tree->left, env, operand, status);
	explore_tree(tree->right, env, operand, status);
}