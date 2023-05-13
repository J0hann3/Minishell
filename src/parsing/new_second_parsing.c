/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_second_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:01:14 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/13 18:16:22 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_instructions(t_instruction *instruc)
{
	size_t	i;

	if (!instruc)
		return ;
	if (instruc->infile >= 0)
		close(instruc->infile);
	if (instruc->outfile >= 0)
		close(instruc->outfile);
	if (instruc->command)
	{
		i = 0;
		while (instruc->command[i])
			free(instruc->command[i++]);
		free(instruc->command);
	}
	free(instruc);
}

void	ft_print(char *ctxt, t_char *expanded_command)
{
	int i = 0;
	printf("%s : \n", ctxt);
	while (expanded_command[i].c)
	{
		printf("%c		%d\n", expanded_command[i].c, expanded_command[i].is_inter);
		i++;
	}
	printf("fin\n");
}

t_instruction	*second_parsing(char *input, size_t command_size, t_env_info *env_info, int fd_heredocs)
{
	t_char			*expanded_command;
	t_char			**command;
	bool			is_ambigous;
	t_instruction	*instruc;
	size_t			i;

	is_ambigous = false;
	instruc = ft_calloc(1, sizeof(t_instruction));
	if (!instruc)
		return (mem_exh("token creation"), NULL);
	instruc->infile = -2;
	instruc->outfile = -2;
	instruc->s_infile = -2;
	instruc->s_outfile = -2;
	if(!expand_heredocs(&fd_heredocs, env_info))
		return (free_instructions(instruc), g_error = 1, NULL);
	expanded_command = expand_dollars(input, command_size, env_info, &is_ambigous);
	if (!expanded_command || is_ambigous)
		return (free_instructions(instruc), free(expanded_command), g_error = 1, NULL);

	if (!expand_wild(&expanded_command))
		return (free_instructions(instruc), free(expanded_command), g_error = 1, NULL);
	
	if (!open_all_fds(instruc, expanded_command, fd_heredocs))
		return (free_instructions(instruc), free(expanded_command), g_error = 1, NULL);
	command = ft_split_quote(expanded_command, (int *)&command_size);
	free(expanded_command);
	if (!command)
		return (free_instructions(instruc), g_error = 1, mem_exh("token creation"), NULL);
	instruc->command = ft_calloc(command_size + 1, sizeof(char *));
	if (!instruc->command)
		return (free_instructions(instruc), free_all(command), mem_exh("token creation"), g_error = 1, NULL);
	
	i = 0;
	while (command[i])
	{
		instruc->command[i] = remove_quotes(command[i]);
		if (instruc->command[i] == NULL)
			return (free_instructions(instruc), free_all(command), g_error = 1, NULL);
		i++;
	}
	free_all(command);
	return (instruc);
}
