/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/26 14:43:12 by jvigny           ###   ########.fr       */
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
 * @brief create and fork for execution
 * child (pid==0)-> execute command with the good redirection
 * parent (pid > 0) -> wait
 * 
 * @param tree 	command to execute in the tree
 * @param env 	write error in env if necessary
 * @param m_b 	meta character before command to exec
 * @param m_n 	meta character after command to exec
 * @param stat 	error value of the last command executed
 * @return void
 */
void	multi_pipe(t_ast *tree, t_env_info *env, enum e_meta_character m_b, enum e_meta_character m_n)
{
	t_instruction	*arg;
	int				stat;
	int				pid;
	int				fildes[2];
	static int		fd_tmp = 0;
	
	if ((m_b == e_and && g_error != 0) || (m_b == e_or && g_error == 0))
		return ;
	if (m_b == e_pipe && fd_tmp == 0)
		return ;
	if (m_n == e_pipe )
		if (pipe(fildes) != 0)
			return (g_error = 1, (void)0);
	g_error = 0;
	pid = fork();
	if (pid == -1)
		return (g_error = 1, (void)0);
	if (pid == 0)
	{
		none_interactive(env->act);
		if (fd_tmp != 0)
		{
			dup2(fd_tmp, STDIN_FILENO);
			close(fd_tmp);
		}
		if (m_n == e_pipe)
		{
			dup2(fildes[1], STDOUT_FILENO);
			close(fildes[0]);
			close(fildes[1]);
		}
		arg = second_parsing(tree->command, tree->size, env);
		exec(arg, env, 1);
		free(arg);
		free_env(env);
		exit(g_error);
	}
	if (fd_tmp != 0)
		close(fd_tmp);
	if (m_n == e_pipe)
	{
		ign_signals(env->act);
		close(fildes[1]);
		fd_tmp = fildes[0];
	}
	else
		add_error_signals(env->act);
	if (m_b == e_pipe && m_n != e_pipe)
	{
		waitpid(pid, &stat, 0);
		new_line_signals(env->act);
		if (WIFEXITED(stat))
		{
			if (WEXITSTATUS(stat) != 0) 
				g_error = WEXITSTATUS(stat);
		}
		while (pid > 0)
		{
			pid = waitpid(-1, &stat, 0);
		}
		reset_signals(env->act);
		fd_tmp = 0;
	}
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
		arg = second_parsing(tree->command, tree->size, env);
		// printf("Error : %d	inst: %p\n", g_error, arg);
		exec(arg, env, 0);
		free(arg);
		return (meta_next);
	}
	else if (meta_before == e_and)
	{
		if (g_error == 0)
		{
			arg = second_parsing(tree->command, tree->size, env);
			exec(arg, env, 0);
			free(arg);
			return (meta_next);
		}
	}
	else if (meta_before == e_or)
	{
		if (g_error != 0)
		{
			arg = second_parsing(tree->command, tree->size, env);
			exec(arg, env, 0);
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
	}
	else if (tree->command != NULL)
		meta_before = skip_or_exec_command(tree, env, meta_before);
}
