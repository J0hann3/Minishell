/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_name.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 18:34:50 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/19 18:37:56 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*ft_strjoin_nbr(char *str, int nbr)
{
	char	*res;
	char	*number_str;
	size_t	i;
	size_t	size1;
	size_t	size2;

	number_str = ft_itoa(nbr);
	if (!number_str)
		return (NULL);
	size1 = ft_strlen(str);
	size2 = ft_strlen(number_str);
	res = ft_calloc(size1 + size2 + 1, sizeof(char));
	if (!res)
		return (free(number_str), NULL);
	i = -1;
	while (++i < size1)
		res[i] = str[i];
	i = -1;
	while (++i < size2)
		res[i + size1] = number_str[i];
	return (res);
}

static char	*get_next_number(char *previous, int nbr)
{
	char	*start;

	start = previous;
	while (*previous != '_')
		previous++;
	if (ft_strlen(previous) > 55)
		return (free(start), NULL);
	previous[1] = 0;
	previous = ft_strjoin_nbr(start, nbr);
	free(start);
	return (previous);
}

char	*get_random_name(void)
{
	char	*file_name;
	int		exist;
	int		nbr;

	nbr = 1;
	file_name = ft_strdup("/tmp/.heredoc_0");
	if (!file_name)
		return (mem_exh("heredocs"), NULL);
	exist = access("/tmp", F_OK);
	if (exist != 0)
		return (free(file_name), ft_write_error("heredocs", "tmp",
				"No such file or directory"), NULL);
	exist = 1;
	while (true)
	{
		exist = access(file_name, F_OK);
		if (exist == 0)
			file_name = get_next_number(file_name, nbr);
		else
			return (file_name);
		if (!file_name)
			return (mem_exh("heredocs"), NULL);
		nbr++;
	}
}
