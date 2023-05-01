/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_here.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:42:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/01 17:10:32 by jvigny           ###   ########.fr       */
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

int	prompt_here(char *ender, int fd, t_env_info *env)
{
	char	*input;
	int		pid;
	int		stat;

	// open tmp file anyway
	// printf(": %s\n", file_name);
	if (!isatty(STDIN_FILENO) && !isatty(STDERR_FILENO))
		return (close(fd), 2); // not interactive
	input = (char *)1;
	pid = fork();
	if (pid == -1)
		return (2);
	if (pid == 0)
	{
		add_error_signals(env->act);
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
			write(fd, "\n", 1);
		}
		close(fd);
		exit(g_error);
	}
	else
	{
		ign_signals(env->act);
		waitpid(pid, &stat, 0);
		reset_signals(env->act);
		if (WIFSIGNALED(stat))
			stat = 128 + WTERMSIG(stat);
		else
			stat = WEXITSTATUS(stat);
		close(fd);
	}
	return (stat);
}