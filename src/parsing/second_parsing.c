/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 19:45:47 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/29 17:27:30 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static void	print_instruc(t_instruction *instruc) // tmp
// {
// 	int	i;

// 	i = 0;
// 	printf("Commands :\n");
// 	while (instruc->command[i])
// 		printf("`%s`\n", instruc->command[i++]);
// 	printf("Infile  : '%d'\n", instruc->infile);
// 	printf("Outfile : '%d'\n", instruc->outfile);
// }

static void	free_instructions(t_instruction *instruc)
{
	size_t	i;

	if (!instruc)
		return ;
	if (instruc->infile > 0)
		close(instruc->infile);
	if (instruc->outfile > 0)
		close(instruc->outfile);
	if (instruc->outerror > 0)
		close(instruc->outerror);
	if (instruc->command)
	{
		i = 0;
		while (instruc->command[i])
			free(instruc->command[i++]);
		free(instruc->command);
	}
	free(instruc);
}

t_instruction	*second_parsing(char *input, size_t command_size, t_env_info *env_info, int fd_heredocs)
{
	char			*expanded_command;
	bool			is_ambigous;
	t_instruction	*instruc;
	size_t			i;

	is_ambigous = false;
	instruc = ft_calloc(1, sizeof(t_instruction));
	if (!instruc)
		return (printf("MALLOC\n"), NULL);
	// expand heredocs
	expanded_command = expand_dollars(input, command_size, env_info, &is_ambigous);
	// printf("command before: '%s'\n", expanded_command);
	if(!(expanded_command && !is_ambigous && open_all_fds(instruc, expanded_command, fd_heredocs))) // changer error
		return (free(expanded_command), free_instructions(instruc), g_error = 1, NULL);
	// expand *
	// printf("command after: '%s'\n", expanded_command);
	instruc->command = ft_split_quote(expanded_command, ' ');
	if (!instruc->command)
		return (NULL);
	i = 0;
	while (instruc->command[i])
		remove_quotes(instruc->command[i++]);
	free(expanded_command);
	return (instruc);
}
