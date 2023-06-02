/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_here.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:42:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 18:50:11 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_warning_message(char *ender)
{
	char	*res;

	res = ft_strjoin3("here-document delimited by end-of-file (wanted `",
			ender, "')");
	if (!res)
		mem_exh("heredocs");
	return (res);
}

void	free_on_crash(char *ender, int fd)
{
	static char	*stck_ender = NULL;
	static int	stck_fd = 0;

	if (!ender && stck_ender && stck_fd)
	{
		free(stck_ender);
		close(stck_fd);
		stck_ender = NULL;
		stck_fd = 0;
	}
	else
	{
		stck_ender = ender;
		stck_fd = fd;
	}
}

void	child(char *ender, int fd, t_env_info *env)
{
	char	*input;

	(get_next_line(-1), free_on_crash(ender, fd));
	close_fd_heredocs(env);
	add_error_signals(env->act);
	free_env(env);
	input = (char *)1;
	while (input)
	{
		input = readline("> ");
		if (eq(ender, input))
			break ;
		else if (!input)
		{
			input = get_warning_message(ender);
			ft_write_error("warning", NULL, input);
			break ;
		}
		write(fd, input, ft_strlen(input));
		free(input);
		write(fd, "\n", 1);
	}
	(free(ender), free(input));
	close(fd);
	exit(EXIT_SUCCESS);
}

int	prompt_here(char *ender, int fd_w, t_env_info *env)
{
	int		pid;
	int		stat;

	if (!isatty(STDIN_FILENO) || !isatty(STDERR_FILENO))
		return (close(fd_w), 0);
	pid = fork();
	if (pid == -1)
		return (ft_write_error("heredocs", NULL,
				strerror(errno)), close(fd_w), 2);
	if (pid == 0)
		child(ender, fd_w, env);
	else
	{
		ign_signals(env->act);
		if (waitpid(pid, &stat, 0) == -1)
			ft_write_error("heredocs", NULL, strerror(errno));
		reset_signals(env->act);
		if (WIFSIGNALED(stat))
			stat = 128 + WTERMSIG(stat);
		else
			stat = WEXITSTATUS(stat);
		close(fd_w);
	}
	return (stat);
}
