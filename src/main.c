/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 18:31:30 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/04 10:45:59 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

bool	has_error_for_meta(char *input, size_t i)
{
	if (input[i] == '&' && input[i + 1] != '&')
		return (0);
	else if (input[i] == '<' || input[i] == '>')
	{
		if (!has_argument_right(input + i))
			return (1);
	}
	else if (input[i] == '|' || input[i] == '&')
	{
		if (!has_argument_left(input, (input + i))
			|| !has_argument_right(input + i))
			return (1);
	}
	return (0);
}

void	remove_multiple_wspaces(char *input)
{
	char	*dest;
	char	*start;
	size_t	space;

	space = 1;
	dest = input;
	start = input;
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			space = *input;
			*dest++ = *input++;
			while (*input != (char)space)
				*dest++ = *input++;
		}
		space = ((space + 1) * (*input++ == ' ' || *(input - 1) == '\t'));
		if (space <= 1)
			*dest++ = *(input - 1) + ((' ' - '\t') * (*(input - 1) == '\t'));
	}
	*dest = 0;
	while ((*dest == ' ' || !*dest) && dest != start)
		*(dest--) = 0;
}

int	syntax_errors(char *input)
{
	size_t	i;

	if (quotes_not_closed(input))
		return (2);
	if (has_parenthesis_not_closed(input))
		return (2);
	remove_useless_parenthesis(&input);
	if (has_error_on_operators_and_parenthesis(input))
		return (2);
	remove_multiple_wspaces(input);
	printf("canon : `%s`\n", input);
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		if ((is_meta_character(input[i])) && has_error_for_meta(input, i))
			return (2);
		i++;
	}
	return (0);
}

void	free_instructions(t_instruction *instruc)
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

void	print_instruc(t_instruction *instruc) // tmp
{
	int	i;

	i = 0;
	printf("Commands :\n");
	while (instruc->command[i])
		printf("'%s'\n", instruc->command[i++]);
	printf("Infile  : '%d'\n", instruc->infile);
	printf("Outfile : '%d'\n", instruc->outfile);
}

t_instruction	*called_by_jo_from_tree(char *input, char **env) //tmp
{
	char			*expanded_command;
	t_env_info		env_info = {0, 0, env};
	t_instruction	*instruc;
	
	instruc = ft_calloc(1, sizeof(t_instruction));
	if (!instruc)
		return (NULL);
	expanded_command = expand_dollars(input, ft_strlen(input), &env_info);
	// expand *
	if(!(expanded_command && open_all_fds(instruc, expanded_command)))
	{
		free_instructions(instruc);
		return (free(expanded_command), free_instructions(instruc), NULL);
	}
	instruc->command = ft_split(expanded_command, ' ');
	print_instruc(instruc);
	free(expanded_command);
	return (instruc);
}

int	main(int argc, char *argv[], char *env[])
{
	char			*input;
	char			*tmp;
	int				ret_err;
	t_instruction	*instruc;

	(void)argc;
	(void)argv;
	(void)env;
	input = (char *)1;
	ret_err = 0;
	while (input != NULL)
	{
		input = readline("minishell$> ");
		if (!input)
			break ;
		add_history(input);
		ret_err = syntax_errors(input);
		if (!ret_err)
		{
			create_tree(input);
			instruc = called_by_jo_from_tree(input, env);
			free_instructions(instruc);
		}
		else
			printf("Erreur de syntax\n");
		free(input);
	}
	rl_clear_history();
	return (ret_err);
}
