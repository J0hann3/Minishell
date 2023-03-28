/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:45:34 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/28 15:55:59 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

// static t_ast	*find_right_command(t_ast *node)
// {
// 	while (node != NULL)
// 	{
// 		if (node->parent != NULL && node->parent->command != NULL)
// 			return(node->parent);
// 		node = node->parent;
// 	}
// 	return (NULL);
// }

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

/**
 * @brief create and fork for execution
 * child (pid==0)-> write permission
 * parent (pid > 0) -> read permission and wait
 * 
 * @param env 	write error in env if necessary
 * @return int : pid of process on success, else -1
 */
int	ft_pipe(t_env_info *env, int fildes[2])
{
	int	pid;
	

	if (pipe(fildes) != 0)
	{
		env->error = 1;
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		env->error = 1;
		return (-1);
	}
	else if (pid == 0)
	{
		close(fildes[0]);
		printf("[%d]close test : %d\n", pid, fildes[0]);
		close(STDOUT_FILENO);
		// if (dup2(fildes[1], STDOUT_FILENO) == -1)
		// 	return (perror("Error"), -1);
		// close(fildes[1]);
		// printf("[%d]close : %d\n", pid, fildes[1]);
		// printf("test----------------\n");
		return (13);
	}
	else
	{
	// 	close(fildes[1]);
	// 	printf("[%d]close : %d\n", pid, fildes[1]);
		return (pid);
	}
}

int	multi_pipe(t_ast *tree, t_env_info *env, enum e_meta_character m_b, enum e_meta_character m_n, int stat)
{
	t_instruction			arg;
	int						pid;
	static int				fildes[2];
	static int				fd_tmp = -1;


	printf("-----------pipe----------[%d][%d][%d]\n", fildes[0], fildes[1], fd_tmp);
	if (m_n == e_pipe)
	{
		pid = ft_pipe(env, fildes);
		printf("PID		[%d][%d][%d] : %d\n", fildes[0], fildes[1], fd_tmp, pid);
		if (pid == -1)
			return(printf("error pipe\n"), -1);
		if (pid == 0)
		{
			if ((m_b == e_and && stat != 0) || (m_b == e_or && stat == 0))
			{
				printf("don't execute command\n");
				printf("[%d]close : %d\n", pid, fildes[1]);
				close(fildes[1]);
				exit(0);
			}
			// if (fd_tmp != -1)
			// 	dup2(fd_tmp, STDIN_FILENO);
			// arg.command = ft_split(tree->command, ' ');
			// stat = exec(&arg, env);
			printf("%s\n", tree->command);
			printf("[%d]close : %d\n", pid, fildes[1]);
			close(fildes[1]);
			exit(0);				// maybe need to free malloc ?
		}
		else
		{
			waitpid(pid, &stat, 0);
			if (WIFEXITED(stat))
				stat = WEXITSTATUS(stat);
			else if (WIFSIGNALED(stat))
			{
				printf("signal\n");
				stat = WTERMSIG(stat);
			}
			else if (WIFSTOPPED(stat))
			{
				printf("stop\n");
				stat = WSTOPSIG(stat);
			}
			printf("Wait	status: %d\n", stat);
			printf("fd_tmp = %d\n", fildes[0]);
			fd_tmp = fildes[0];
			// close(fd_tmp);
		}
	}
	else if (m_b == e_pipe)
	{
		// dup2(fd_tmp, STDIN_FILENO);
		printf("%s\n", tree->command);
		// arg.command = ft_split(tree->command, ' ');
		// stat = exec(&arg, env);
		printf("[%d]close : %d\n", pid, fd_tmp);
		close(fd_tmp);
		fd_tmp = -1;
	}
	printf("---------------------\n");
	return (stat);
}

static enum e_meta_character	skip_or_exec_command(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat)
{
	enum e_meta_character	meta_next;
	t_instruction 			arg;

	meta_next = find_next_meta(tree);
	printf("meta_before : %d		meta_next: %d		stat:%d\n",meta_before, meta_next, stat);
	if (meta_next == e_pipe || meta_before == e_pipe)		//need to skip command if and or or
		stat = multi_pipe(tree, env, meta_before, meta_next,stat);
	else if (meta_before == e_empty)
	{
		printf("%s\n", tree->command);
		// arg.command = ft_split(tree->command, ' ');
		// stat = exec(&arg, env);
	}
	else if (meta_before == e_and)
	{
		if (stat == 0)
		{
			printf("%s\n", tree->command);
			// arg.command = ft_split(tree->command, ' ');		//replace by parsing2
			// stat = exec(&arg, env);
		}
	}
	else if (meta_before == e_or)
	{
		if (stat != 0)
		{
			printf("%s\n", tree->command);
			// arg.command = ft_split(tree->command, ' ');
			// stat = exec(&arg, env);
		}
	}
	return (meta_next);
}

void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character *meta_before, int stat)
{
	// printf("NODE\n");
	// printf("meta_befor_p:%p\n", meta_before);
	// if (meta_before != NULL)
	// 	printf("meta_before_d:%d\n", *meta_before);

	if (tree == NULL)
	{
		// printf("----------------------------\n");
		return;
	}
	
	if (tree->left == NULL && tree->right == NULL && tree->command != NULL)
	{
		*meta_before = skip_or_exec_command(tree, env, *meta_before, stat);

		// printf("meta_before_p:%p\n", meta_before);
		// if (meta_before != NULL)
		// 	printf("meta_before_d:%d\n", *meta_before);
		// printf("stat: %d\n", stat);
		// if (tree->meta != e_empty)
		// 	printf("tree block:%d\n", tree->meta);
		// if (tree->command != NULL)
		// 	printf("tree_block:%s\n", tree->command);
			
		// printf("----------------------------\n");
		return ;
	}
	// printf("stat: %d\n", stat);
	// if (tree->meta != e_empty)
	// 	printf("tree block:%d\n", tree->meta);
	// if (tree->command != NULL)
	// 	printf("tree_block:%s\n", tree->command);
	// printf("----------------------------\n");
	
	explore_tree(tree->left, env, meta_before, stat);
	explore_tree(tree->right, env, meta_before, stat);
}
