/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/26 21:55:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	find_next_meta(t_ast *node)
{
	return (e_or);
}

static int	exec_command(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	meta_next;
	t_instruction 			*arg;
	int						pid;
	int						fildes[2];

	meta_next = find_meta_next(tree);
	if (meta_before == e_pipe)
	{
		if (meta_next == e_pipe)
		{
			pid = ft_pipe(env, fildes);
			if (pid == 0)
			{
				arg->command = ft_split(tree->command, ' ');
				stat = exec(&arg, env);
				close(fildes[1]);
				exit(stat);				// maybe need to free malloc ?
			}
			else
			{
				waitpid(pid, &stat, 0);
				if (WIFEXITED(stat))
					stat = WEXITSTATUS(stat);
				else 
					stat = 1;				//need to modifie also env->error	no sure ??
			}
		}
		else
		{
			arg->command = ft_split(tree->command, ' ');
			stat = exec(&arg, env);
			// close(fildes[0]);			//don't have the fd to close it
		}
	}
	if (meta_next == e_pipe)
	{
		pid = ft_pipe(env, fildes);
		if (pid == 0)
		{
			arg->command = ft_split(tree->command, ' ');
			stat = exec(&arg, env);
			close(fildes[1]);
			exit(stat);				// maybe need to free malloc ?
		}
		else
		{
			waitpid(pid, &stat, 0);
			if (WIFEXITED(stat))
				stat = WEXITSTATUS(stat);
			else 
				stat = 1;				//need to modifie also env->error	no sure ??
		}
	}
	if (meta_before == e_and)
	{
		if (stat == 0)
		{
			arg->command = ft_split(tree->command, ' ');		//replace by parsing2
			stat = exec(&arg, env);
		}
		
	}
	else if (meta_before == e_or)
	{
		if (stat != 0)
		{
			arg->command = ft_split(tree->command, ' ');
			stat = exec(&arg, env);
		}
	}
	meta_before = meta_next;
	return (stat);
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