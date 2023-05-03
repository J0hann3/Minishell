/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/03 16:17:18 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static enum e_meta_character	find_next_meta(t_ast *node)
{
	enum e_meta_character	tmp;
	
	while (node != NULL)
	{
		if (node->parent != NULL && node->parent->meta != e_empty)
		{
			tmp = node->parent->meta;
			node->parent->meta = e_empty;
			return (tmp);
		}
		node = node->parent;
	}
	return (e_empty);
}

/**
 * @brief exec command if needed or return
 * 
 * @param tree command to exec in tree
 * @param env write error in env if necessary
 * @param meta_before meta character before command to exec
 * @param stat error value of the last command executed
 * @return enum e_meta_character : meta character for the next command
 */
static enum e_meta_character	skip_or_exec_command(t_ast *tree, t_env_info *env, enum e_meta_character meta_before)
{
	enum e_meta_character	meta_next;
	t_instruction 			*arg;

	meta_next = find_next_meta(tree);
	if (g_error == 130 && meta_before != e_empty_new)
		return (meta_next);
	if (meta_next == e_pipe || meta_before == e_pipe)
	{
		multi_pipe(tree, env, meta_before, meta_next);
		return (meta_next);
	}
	else if (meta_before == e_empty || meta_before == e_empty_new)
	{
		arg = second_parsing(tree->command, tree->size, env, tree->fd_heredocs);
		exec(arg, env);
		free(arg);
		return (meta_next);
	}
	else if (meta_before == e_and)
	{
		if (g_error == 0)
		{
			arg = second_parsing(tree->command, tree->size, env, tree->fd_heredocs);
			exec(arg, env);
			free(arg);
			return (meta_next);
		}
	}
	else if (meta_before == e_or)
	{
		if (g_error != 0)
		{
			arg = second_parsing(tree->command, tree->size, env, tree->fd_heredocs);
			exec(arg, env);
			free(arg);
			return (meta_next);
		}
	}
	return (meta_before);
}
/**
 * @brief explore the tree to exec command if needed or skip branch or command
 * 
 * @param tree 
 * @param env 
 * @param meta_before meta character before command to exec
 * @param stat error value of the last command executed
 */
void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character meta_before)
{
	enum e_meta_character	tmp;

	if (tree == NULL)
		return;
	if (g_error == 130 && meta_before != e_empty_new)
		return ;
	if (tree->meta != e_empty)
	{
		tmp = tree->meta;
		if (tree->left != NULL && tree->left->command != NULL)
			meta_before = skip_or_exec_command(tree->left, env, meta_before);
		else
			explore_tree(tree->left, env, meta_before);
		meta_before = tmp;
		if (tree->right != NULL && tree->right->command != NULL)
			meta_before = skip_or_exec_command(tree->right, env, meta_before);
		else if (meta_before == e_and)
		{
			if (g_error == 0)
				explore_tree(tree->right, env, meta_before);
		}
		else if (meta_before == e_or)
		{
			if (g_error != 0)
				explore_tree(tree->right, env, meta_before);
		}
		else
			explore_tree(tree->right, env, meta_before);
	}
	else if (tree->command != NULL)
		meta_before = skip_or_exec_command(tree, env, meta_before);
}
