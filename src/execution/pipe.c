/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:16:55 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/27 13:37:00 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static void	redirect_infile(int fd, t_env_info *env)
{
	if (fd != 0)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			free_env(env);
			g_error = 1;
			close(fd);
			ft_write_error("pipe", NULL, strerror(errno));
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
}

static void	redirect_outfile(t_env_info *env, int fd[2],
		enum e_meta_character meta_next)
{
	if (meta_next == e_pipe)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			free_env(env);
			g_error = 1;
			close(fd[0]);
			close(fd[1]);
			ft_write_error("pipe", NULL, strerror(errno));
			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		close(fd[1]);
	}
}

static void	pipe_waitpid(t_env_info *env, int pid, int *fd)
{
	int				stat;

	if (waitpid(pid, &stat, 0) == -1)
	{
		g_error = 1;
		ft_write_error("pipe", NULL, strerror(errno));
	}
	if (WIFSIGNALED(stat))
		g_error = 128 + WTERMSIG(stat);
	else
		g_error = WEXITSTATUS(stat);
	while (stat > 0)
	{
		stat = waitpid(-1, NULL, 0);
	}
	reset_signals(env->act);
	*fd = 0;
}

static void	pipe_close_fd(int *fd, int fildes[2], int fd_heredoc,
		enum e_meta_character m_n)
{
	if (fd_heredoc >= 0)
		close(fd_heredoc);
	if (*fd != 0)
		close(*fd);
	if (m_n == e_pipe)
	{
		close(fildes[1]);
		*fd = fildes[0];
	}
}

static void	execution(t_ast *tree, t_env_info *env)
{
	t_instruction *inst;

	inst = second_parsing(tree->command, tree->size, env, tree->fd_heredocs);
	free(find_most_left_command(env->tree));
	exec(inst, env);
	tree->fd_heredocs = -1;
	free_env(env);
	exit(g_error);
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
 * @return void
 */
void	multi_pipe(t_ast *tree, t_env_info *env, enum e_meta_character m_b,
		enum e_meta_character m_n)
{
	int				pid;
	int				fildes[2];
	static int		fd_tmp = 0;

	if ((m_b == e_and && g_error != 0) || (m_b == e_or && g_error == 0)
		|| (m_b == e_pipe && fd_tmp == 0))
		return ;
	if (m_n == e_pipe && pipe(fildes) != 0)
		return (g_error = 1, ft_write_error("pipe", NULL, strerror(errno)));
	g_error = 0;
	pid = fork();
	if (pid == -1)
		return (g_error = 1, close(fildes[0]), close(fildes[1]),
			ft_write_error("pipe", NULL, strerror(errno)));
	if (pid == 0)
	{
		get_next_line(-1);
		none_interactive(env->act);
		redirect_infile(fd_tmp, env);
		redirect_outfile(env, fildes, m_n);
		execution(tree, env);
	}
	ign_signals(env->act);
	pipe_close_fd(&fd_tmp, fildes, tree->fd_heredocs, m_n);
	if (m_b == e_pipe && m_n != e_pipe)
		pipe_waitpid(env, pid, &fd_tmp);
}
