/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 18:29:50 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:16:35 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_ambigous_redirect(char *input_redir)
{
	char	*tmp;

	while (*input_redir != '$')
		input_redir++;
	input_redir++;
	tmp = ft_strndup(input_redir - 1, get_size_of_var(input_redir) + 1);
	if (!tmp)
		return (mem_exh("expand dollar"));
	ft_write_error(NULL, tmp, "ambigous redirect");
	free(tmp);
}

static bool	is_ambig_redir(char *input, int index)
{
	int	i;

	i = get_size_of_var(input + 1) + 1;
	if (input[i] && !is_wspace(input[i]))
		return (false);
	i = 1;
	while (i < index)
	{
		if (*(input - i) == '>')
			return (print_ambigous_redirect(input), true);
		if (*(input - i) == '<')
		{
			if (i + 1 < index && *(input - i - 1) == '<')
				return (false);
			print_ambigous_redirect(input);
			return (true);
		}
		else if (is_wspace(*(input - i)))
			i++;
		else
			return (false);
	}
	return (false);
}

static char	*expand2(t_env_info *env_info, int env_index)
{
	char	*tmp;
	size_t	j;

	tmp = env_info->env[env_index];
	j = 0;
	while (tmp[j] && tmp[j] != '=')
		j++;
	tmp = ft_strdup(tmp + j + 1);
	if (!tmp)
		return (mem_exh("dollar expand"), NULL);
	return (tmp);
}

/**
 * @brief Returns the expanded $ variable, first input character is the
 * very next character after $
 * Forward the i index by the size of the variable
 * 
 * @param input 
 * @param i 
 * @param env_info 
 * @param is_ambigous 
 * @return char* 
 */
char	*expand_dol(char *input, int *i,
				t_env_info *env_info, bool *is_ambigous)
{
	char	*tmp;
	int		env_index;
	size_t	size;

	if (*input == '?')
		return ((*i)++, ft_itoa(g_error));
	size = get_size_of_var(input);
	*i += size;
	tmp = ft_strndup(input, size);
	if (!tmp)
		return (NULL);
	env_index = ft_getenv(env_info->env, tmp);
	free(tmp);
	if (env_index == -1)
	{
		if (is_ambig_redir(input - 1, *i - size))
			*is_ambigous = true;
		return (ft_calloc(1, sizeof(char)));
	}
	tmp = expand2(env_info, env_index);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0' && is_ambig_redir(input - 1, *i - size))
		*is_ambigous = true;
	return (tmp);
}
