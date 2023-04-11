/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 19:45:47 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/11 22:09:14 by qthierry         ###   ########.fr       */
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

t_instruction	*second_parsing(char *input, size_t command_size, t_env_info *env_info)
{
	char			*expanded_command;
	t_instruction	*instruc;
	size_t			i;
	
	instruc = ft_calloc(1, sizeof(t_instruction));
	if (!instruc)
		return (NULL);
	// heredocs
	expanded_command = expand_dollars(input, command_size, env_info);
	if(!(expanded_command && env_info->error != 1 && open_all_fds(instruc, expanded_command))) // changer error
		return (free(expanded_command), free_instructions(instruc), NULL);
	// expand *
	instruc->command = ft_split_quote(expanded_command, ' ');
	if (!instruc->command)
		return (NULL);
	i = 0;
	while (instruc->command[i])
		remove_quotes(instruc->command[i++]);
	// print_instruc(instruc);
	free(expanded_command);
	return (instruc);
}
