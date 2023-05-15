/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:16:55 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/15 18:45:43 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
void	multi_pipe(t_ast *tree, t_env_info *env, enum e_meta_character m_b,
		enum e_meta_character m_n)
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
	if (m_n == e_pipe)
		if (pipe(fildes) != 0)
			return (g_error = 1, ft_write_error("pipe", NULL, strerror(errno)));
	g_error = 0;
	pid = fork();
	if (pid == -1)
		return (g_error = 1, close(fildes[0]), close(fildes[1]),
			ft_write_error("pipe", NULL, strerror(errno)));
	if (pid == 0)
	{
		none_interactive(env->act);
		if (fd_tmp != 0)
		{
			if (dup2(fd_tmp, STDIN_FILENO) == -1)
			{
				free_env(env);
				g_error = 1;
				close(fd_tmp);
				ft_write_error("pipe", NULL, strerror(errno));		//attention close fd == sigpipe else leaks fd
				exit(EXIT_FAILURE);
			}
			close(fd_tmp);
		}
		if (m_n == e_pipe)
		{
			if (dup2(fildes[1], STDOUT_FILENO) == -1)
			{
				free_env(env);
				(g_error = 1, close(fildes[0]), close(fildes[1]));
				ft_write_error("pipe", NULL, strerror(errno));
				exit(EXIT_FAILURE);
			}
			close(fildes[0]);
			close(fildes[1]);
		}
		arg = second_parsing(tree->command, tree->size, env, tree->fd_heredocs);
		tree->fd_heredocs = -1;
		exec(arg, env);
		free(arg);
		free_env(env);
		exit(g_error);
	}
	if (tree->fd_heredocs >= 0)
		close(tree->fd_heredocs);
	ign_signals(env->act);
	if (fd_tmp != 0)
		close(fd_tmp);
	if (m_n == e_pipe)
	{
		close(fildes[1]);
		fd_tmp = fildes[0];
	}
	if (m_b == e_pipe && m_n != e_pipe)
	{
		if (waitpid(pid, &stat, 0) == -1)
			(g_error = 1, ft_write_error("pipe", NULL, strerror(errno)));
		if (WIFSIGNALED(stat))
			g_error = 128 + WTERMSIG(stat);
		else
			g_error = WEXITSTATUS(stat);
		while (pid > 0)
		{
			pid = waitpid(-1, &stat, 0);
		}
		reset_signals(env->act);
		fd_tmp = 0;
	}
}
