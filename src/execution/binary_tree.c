/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/26 12:55:12 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	*find_next_meta(t_ast *node)
{
	return (&e_or);
}

static int	exec_command(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	*meta_next;

	meta_next = find_meta_next(tree);
	if (meta_next == e_pipe)
	{
		pid = ft_pipe(env);			//ATTENTION need to close fd after use
		// if (pid == 0)
		// {
		// 	arg.command = ft_split(tree->command, ' ');
		// 	exec(&arg, env);
		// }
		// else
		// {
		// 	waitpid(pid, &stat, 0);
		// 	// if (WIFEXITED(stat))
		// 	// 	env->error = WEXITSTATUS(stat);
		// 	// else 
		// 	// 	env->error = 1;
		// 	explore_tree(tree->right, env, e_pipe_right, 0);
		// }
	}
	if (meta_before == e_and)
	{
		if (stat == e_true)
		{
			arg.command = ft_split(tree->command, ' ');
			exec(&arg, env);
		}
		else
			return ; 
		
	}
	else if (meta_before == e_or)
	{
		if (stat == e_false)
		{
			arg.command = ft_split(tree->command, ' ');
			exec(&arg, env);
		}
		else
			return ;
	}
	else if (meta_before == e_pipe)
	{
		waitpid(pid, &stat, 0);
		if (WIFEXITED(stat))
			env->error = WEXITSTATUS(stat);
		else 
			env->error = 1;
		arg.command = ft_split(tree->command, ' ');
		exec(&arg, env);
	}
}

void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	meta_next;

	if (tree == NULL)
		return;
	if (tree->left == NULL && tree->right == NULL && tree->command != NULL)
		stat = exec_command(tree, env, meta_before, stat);
	if (tree->meta != e_empty)
		meta_before = tree->meta;
	explore_tree(tree->left, env, meta_before, stat);
	explore_tree(tree->right, env, meta_before, stat);
}