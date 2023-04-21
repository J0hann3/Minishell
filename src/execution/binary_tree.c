/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/21 15:14:27 by jvigny           ###   ########.fr       */
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
void	multi_pipe(t_ast *tree, t_env_info *env, enum e_meta_character m_b, enum e_meta_character m_n, int stat)
{
	t_instruction			*arg;
	int						pid;
	int						fildes[2];
	static int				fd_tmp = 0;

	if (m_b == e_pipe && fd_tmp == 0)
		return ;
	if (m_n == e_pipe )
	{
		if (pipe(fildes) != 0)
		{
			env->error = 1;
			return ;
		}
	}
	pid = fork();
	if (pid == -1)
	{
		env->error = 1;
		return ;
	}
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
		if ((m_b == e_and && stat != 0) || (m_b == e_or && stat == 0))
		{
			close(fildes[1]);
			free_env(env);
			exit(1);
		}
		arg = second_parsing(tree->command, tree->size, env);
		exec(arg, env);
		free_env(env);
		free(arg);
		exit(0);
	}
	else
	{
		ign_signals(env->act);
		if (fd_tmp != 0)
			close(fd_tmp);
		if (m_n == e_pipe)
		{
			close(fildes[1]);
			fd_tmp = fildes[0];
		}
		else
			fd_tmp = 0;
		waitpid(pid, &stat, 0);
		reset_signals(env->act);
		if (WIFEXITED(stat))
		{
			if (WEXITSTATUS(stat) != 0) 
				env->error = WEXITSTATUS(stat);
			if (env->error == 1)
				fd_tmp = 0;
		}
		else if (WIFSIGNALED(stat))
			env->error = 128 + WTERMSIG(stat);
		// printf("Error pipe : %d\n",env->error);
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
static enum e_meta_character	skip_or_exec_command(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	meta_next;
	t_instruction 			*arg;

	meta_next = find_next_meta(tree);
	printf("Error : %d	meta : %d\n", env->error, meta_before);
	if (env->error == 130 && meta_before != e_empty_new)
		return (meta_next);
	if (meta_next == e_pipe || meta_before == e_pipe)
	{
		multi_pipe(tree, env, meta_before, meta_next,stat);
		return (meta_next);
	}
	else if (meta_before == e_empty)
	{
		arg = second_parsing(tree->command, tree->size, env);
		exec(arg, env);
		free(arg);
		return (meta_next);
	}
	else if (meta_before == e_and)
	{
		if (stat == 0)
		{
			arg = second_parsing(tree->command, tree->size, env);
			exec(arg, env);
			free(arg);
			return (meta_next);
		}
	}
	else if (meta_before == e_or)
	{
		if (stat != 0)
		{
			arg = second_parsing(tree->command, tree->size, env);
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
void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	tmp;

	printf("Error : %d	meta : %d\n", env->error == 130, meta_before != e_empty_new);
	if (env->error == 130 && meta_before != e_empty_new)
		return ;
	if (tree == NULL)
		return;
	if (tree->meta != e_empty)
	{
		tmp = tree->meta;
		if (tree->left != NULL && tree->left->command != NULL)
			meta_before = skip_or_exec_command(tree->left, env, meta_before, stat);
		else
			explore_tree(tree->left, env, meta_before, env->error);
		meta_before = tmp;
		if (tree->right != NULL && tree->right->command != NULL)
			meta_before = skip_or_exec_command(tree->right, env, meta_before, env->error);
		else if (meta_before == e_and)
		{
			if (env->error == 0)
				explore_tree(tree->right, env, meta_before, env->error);
		}
		else if (meta_before == e_or)
		{
			if (env->error != 0)
				explore_tree(tree->right, env, meta_before, env->error);
		}
	}
	else if (tree->command != NULL)
		meta_before = skip_or_exec_command(tree, env, meta_before, stat);
}
