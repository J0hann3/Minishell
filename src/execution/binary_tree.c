/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/28 20:00:37 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

enum e_meta_character	find_next_meta(t_ast *node)
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

// /**
//  * @brief create and fork for execution
//  * child (pid==0)-> write permission
//  * parent (pid > 0) -> read permission and wait
//  * 
//  * @param env 	write error in env if necessary
//  * @return int : pid of process on success, else -1
//  */
// int	ft_pipe(t_env_info *env, int fildes[2])
// {
// 	int	pid;
	

// 	if (pipe(fildes) != 0)
// 	{
// 		env->error = 1;
// 		return (-1);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		env->error = 1;
// 		return (-1);
// 	}
// 	else if (pid == 0)
// 	{
// 		close(fildes[0]);
// 		printf("[%d]close test : %d\n", pid, fildes[0]);
// 		close(STDOUT_FILENO);
// 		// if (dup2(fildes[1], STDOUT_FILENO) == -1)
// 		// 	return (perror("Error"), -1);
// 		// close(fildes[1]);
// 		// printf("[%d]close : %d\n", pid, fildes[1]);
// 		// printf("test----------------\n");
// 		return (13);
// 	}
// 	else
// 	{
// 	// 	close(fildes[1]);
// 	// 	printf("[%d]close : %d\n", pid, fildes[1]);
// 		return (pid);
// 	}
// }

void	multi_pipe(t_ast *tree, t_env_info *env, enum e_meta_character m_b, enum e_meta_character m_n, int stat)
{
	t_instruction			arg;
	int						pid;
	int						fildes[2];
	static int				fd_tmp = 0;

	if (m_n == e_pipe )
	{
		if (pipe(fildes) != 0)
		{
			env->error = 1;
			return (-1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		env->error = 1;
		return (-1);
	}
	if (pid == 0)
	{
		if (fd_tmp != 0)
			dup2(fd_tmp, STDIN_FILENO);
		if (m_n == e_pipe)
		{
			dup2(fildes[1], STDOUT_FILENO);
			close(fildes[0]);
		}
		if ((m_b == e_and && stat != 0) || (m_b == e_or && stat == 0))
		{
			printf("don't execute command\n");
			close(fildes[1]);
			exit(0);
		}
		arg.command = ft_split(tree->command, ' ');
		exec(&arg, env);
		if (m_n == e_pipe)
			close(fildes[1]);
		else
		{
			close(fd_tmp);
			fd_tmp = 0;
		}
		exit(0);
	}
	else
	{
		waitpid(pid, &stat, 0);
		if (m_n == e_pipe)
			close(fildes[1]);
		if (WIFEXITED(stat))
			env->error = WEXITSTATUS(stat);
		else if (WIFSIGNALED(stat))
			env->error = 128 + WTERMSIG(stat);
		// else if (WIFSTOPPED(stat))
		// {
		// 	// printf("stop\n");
		// 	stat = WSTOPSIG(stat);
		// }
		fd_tmp = fildes[0];
	}
}

static enum e_meta_character	skip_or_exec_command(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	meta_next;
	t_instruction 			arg;

	meta_next = find_next_meta(tree);
	// printf("meta_before : %d		meta_next: %d		stat:%d\n",meta_before, meta_next, stat);
	if (meta_next == e_pipe || meta_before == e_pipe)
	{
		multi_pipe(tree, env, meta_before, meta_next,stat);
		return (meta_next);
	}
	else if (meta_before == e_empty)
	{
		arg.command = ft_split(tree->command, ' ');
		exec(&arg, env);
		return (meta_next);
	}
	else if (meta_before == e_and)
	{
		if (stat == 0)
		{
			arg.command = ft_split(tree->command, ' ');
			exec(&arg, env);
			return (meta_next);
		}
	}
	else if (meta_before == e_or)
	{
		if (stat != 0)
		{
			arg.command = ft_split(tree->command, ' ');
			exec(&arg, env);
			return (meta_next);
		}
	}
	return (meta_before);
}

void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	tmp;

	if (tree == NULL)
		return;
	if (tree->meta != e_empty)
	{
		tmp = tree->meta;
	}
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
