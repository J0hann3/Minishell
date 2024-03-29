/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:01:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/02 20:18:38 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_instructions(t_instruction *instruc)
{
	size_t	i;

	if (!instruc)
		return ;
	if (instruc->infile >= 0)
		close(instruc->infile);
	if (instruc->outfile >= 0)
		close(instruc->outfile);
	if (instruc->s_infile >= 0)
		close(instruc->s_infile);
	if (instruc->s_outfile >= 0)
		close(instruc->s_outfile);
	if (instruc->command)
	{
		i = 0;
		while (instruc->command[i])
			free(instruc->command[i++]);
		free(instruc->command);
	}
	free(instruc);
}

static t_char	*expand_all(char *input,
	size_t command_size, int *fd_heredocs, t_env_info *env_info)
{
	t_char	*expanded_command;
	bool	is_ambigous;

	is_ambigous = false;
	if (!expand_heredocs(fd_heredocs, env_info))
		return (g_error = 1, NULL);
	expanded_command = expand_dollars(input,
			command_size, env_info, &is_ambigous);
	if (!expanded_command || is_ambigous)
		return (free(expanded_command), g_error = 1, NULL);
	if (!expand_wild(&expanded_command))
		return (free(expanded_command), g_error = 1, NULL);
	return (expanded_command);
}

static bool	fill_instruc_command(t_char *expanded_command,
	t_instruction *instruc, int command_size)
{
	t_char	**command;
	size_t	i;

	command = ft_split_quote(expanded_command, (int *)&command_size);
	free(expanded_command);
	if (!command)
		return (free_instructions(instruc),
			g_error = 1, mem_exh("token creation"), false);
	instruc->command = ft_calloc(command_size + 1, sizeof(char *));
	if (!instruc->command)
		return (free_instructions(instruc), free_all(command),
			mem_exh("token creation"), g_error = 1, false);
	i = 0;
	while (command[i])
	{
		instruc->command[i] = remove_quotes(command[i]);
		if (instruc->command[i] == NULL)
			return (free_instructions(instruc),
				free_all(command), g_error = 1, false);
		i++;
	}
	free_all(command);
	return (true);
}

t_instruction	*second_parsing(char *input,
	size_t command_size, t_env_info *env_info, int fd_heredocs)
{
	t_char			*expanded_command;
	t_instruction	*instruc;

	instruc = ft_calloc(1, sizeof(t_instruction));
	if (!instruc)
		return (mem_exh("token creation"), NULL);
	instruc->infile = -2;
	instruc->outfile = -2;
	instruc->s_infile = -2;
	instruc->s_outfile = -2;
	expanded_command = expand_all(input, command_size, &fd_heredocs, env_info);
	if (!expanded_command)
		return (free_instructions(instruc), NULL);
	if (!open_all_fds(instruc, expanded_command, fd_heredocs))
		return (free_instructions(instruc),
			free(expanded_command), g_error = 1, NULL);
	if (!fill_instruc_command(expanded_command, instruc, command_size))
		return (NULL);
	return (instruc);
}
