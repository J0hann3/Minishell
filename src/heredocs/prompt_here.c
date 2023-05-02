/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_here.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:42:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/02 17:33:28 by qthierry         ###   ########.fr       */
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

void	child(char *ender, int fd, t_env_info *env, char *input)
{
	heredocs_signal(env->act);
	while (input)
	{
		input = readline("> ");
		if (eq(ender, input))
			break ;
		else if (!input)
		{
			input = get_warning_message(ender);
			ft_write_error("warning", NULL, input); // rajouter num ligne si besoin/envie a l'aide
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

	if (!isatty(STDIN_FILENO) || !isatty(STDERR_FILENO))
		return ((void)close(fd_w));
	input = (char *)1;
	pid = fork();
	if (pid == 0)
	{
		close(fd_r);
		child(ender, fd_w, env, input);
	}
	else
	{
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