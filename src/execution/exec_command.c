/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:18:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/21 18:25:57 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtins(char **arg, t_env_info *env)
{
	if (ft_strcmp(arg[1], "echo") != 0)
		return (ft_echo(arg, (const t_env_info *)env), 1);
	if (ft_strcmp(arg[1], "cd") != 0)
		return (ft_cd(arg, env), 1);
	if (ft_strcmp(arg[1], "pwd") != 0)
		return (ft_pwd(arg, env), 1);
	if (ft_strcmp(arg[1], "export") != 0)
		return (ft_export(arg, env), 1);
	if (ft_strcmp(arg[1], "unset") != 0)
		return (ft_unset(arg, env), 1);
	if (ft_strcmp(arg[1], "env") != 0)
		return (ft_env(arg, env), 1);
	if (ft_strcmp(arg[1], "exit") != 0)
		return (ft_exit(arg, env), 1);
	return (0);
}

static int	contain_slash(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '/')
			return (1);
		++i;
	}
	return (0);
}

static char	*explore_path(char *name, char *env_path)
{
	char	**var_path;
	char	*path;
	int		i;

	i = 0;
	var_path = ft_split(env_path, ':');
	while (var_path[i] != NULL)
	{
		path = ft_strjoin(name, var_path[i]);
		if (path == NULL)
		{
			++i;
			continue ;	//code error
		}
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)		//not sure condition F_OX is usefull ?
			return (free_str(var_path), path);
		free(path);
		++i;
	}
	free_str(var_path);
	return (NULL);
}

static char	*find_path_command(char *str, t_env_info *env)
{
	char	*path;
	int		i_path;

	if (contain_slash(str) != 0)
	{
		if (str[0] != '/')
			path = find_absolute_path(str);
		else
			path = ft_strdup(str);
		return (path);
	}
	i_path = ft_getenv(env->env, "PATH");
	if (i_path == -1)
	{
		env->error = 1;			//code error au pif
		return (NULL);
	}
	return (explore_path(str, env->env[i_path]));
}

void	exec(t_instruction *inst, t_env_info *env)
{
	char	*path;
	// int		pid;

	if (inst == NULL || inst->command == NULL)
	{
		env->error = 127;		//code error + need to print error
		return ;
	}
	if (inst->command[0] == NULL)
		return ;
	//need to do redirection now
	if (is_builtins(inst->command, env) != 0)
	{
		return ;
	}
	path = find_path_command(inst->command[0], env);
	if (path == NULL)
	{
		env->error = 127;		//commmand not found
		ft_write_error(NULL, inst->command[0], "command not found");
		return ;
	}
	// pid = fork();
	printf("PATH: %s\n",path);


	free(path);
}