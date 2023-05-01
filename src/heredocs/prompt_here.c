/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_here.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:42:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/01 18:20:08 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_warning_message(char *ender)
{
	char	*res;

	res = ft_strjoin3("here-document delimited by end-of-file (wanted `",
			ender, "')");
	return (res);
}

void	child(char *ender, int fd, t_env_info *env, char *input)
{
	printf("PID CHILD : %d\n", getpid());
	heredocs_signal(env->act);
	while (input)
	{
		input = readline("> ");
		if (eq(ender, input))
			break ;
		else if (!input)
		{
			ft_write_error("warning", NULL, get_warning_message(ender)); // rajouter ligne si besoin ;
			break ;
		}
		write(fd, input, ft_strlen(input));
		free(input);
		write(fd, "\n", 1);
	}
	free(input);
	close(fd);
	free_env(env);
	free(ender);
	exit(EXIT_SUCCESS);
}

void	prompt_here(char *ender, int fd_w, int fd_r, t_env_info *env)
{
	char	*input;
	int		pid;
	int		status;

	// open tmp file anyway
	// printf(": %s\n", file_name);
	if (!isatty(STDIN_FILENO) || !isatty(STDERR_FILENO)) // not interactive
		return ((void)close(fd_w));
	// signaux
	input = (char *)1;
	pid = fork();
	if (pid == 0)
	{
		close(fd_r);
		child(ender, fd_w, env, input);
	}
	else
	{
		printf("PID: %d\n", pid);
		heredocs_error_signal(env->act);
		waitpid(pid, &status, 0);
		reset_signals(env->act);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0) 
				g_error = WEXITSTATUS(status);
		}
		close(fd_w);
	}
}