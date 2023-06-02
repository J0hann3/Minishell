/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 22:46:16 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/24 18:52:54 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	set_input_position(char **input, bool *is_heredoc)
{
	if (**input != *(*input + 1) && is_redirection(*(*input + 1)))
		return (ft_write_error(NULL, NULL,
				get_error_token(*input + 1)), false);
	if (**input == '<' && *(*input + 1) == '<')
		*is_heredoc = true;
	(*input)++;
	if (**input == '>' || **input == '<')
		(*input)++;
	while (is_wspace((**input)))
		(*input)++;
	return (true);
}

static bool	on_heredoc(char *start, t_env_info *env)
{
	int		error;

	if (env->fds_heredocs[env->len_heredocs] != -1)
		close(env->fds_heredocs[env->len_heredocs]);
	error = do_here_docs(start, env);
	if (error == 130)
	{
		if (env->fds_heredocs[0] >= 0)
			close(env->fds_heredocs[0]);
		return (env->fds_heredocs[0] = -2, false);
	}
	else if (error != 0)
		return (false);
	return (true);
}

/**
 * @brief Check if the redirection has a valid argument
 * Forward input at the last charcater of the argument.
 * If no valid argument, write error of the redirection.
 * This function call heredoc open if encounterd.
 * This function can call : 
 * 		is_quote_closed
 * 		do_here_docs
 * @param input 
 * @return true 
 * @return false 
 */
bool	is_redirection_ok(char **input, t_env_info *env)
{
	char	*start;
	bool	is_heredoc;

	is_heredoc = false;
	if (!set_input_position(input, &is_heredoc))
		return (false);
	start = *input;
	while (**input && !is_wspace(**input) && !is_operator((*input))
		&& !is_parenthesis(**input) && !is_redirection(**input))
	{
		if ((**input == '\'' || **input == '"')
			&& !is_quote_closed(input))
			return (false);
		(*input)++;
	}
	if ((*input) == start)
		return (ft_write_error(NULL, NULL, get_error_token(*input)), false);
	(*input)--;
	if (is_heredoc && !on_heredoc(start, env))
		return (false);
	return (true);
}
