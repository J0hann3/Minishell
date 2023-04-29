/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:18:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/29 17:30:55 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_builtins(t_instruction *arg, t_env_info *env)
{
	if (ft_strcmp(arg->command[0], "echo") == 0)
		return (ft_echo(arg->command, (const t_env_info *)env), 1);
	if (ft_strcmp(arg->command[0], "cd") == 0)
		return (ft_cd(arg->command, env), 1);
	if (ft_strcmp(arg->command[0], "pwd") == 0)
		return (ft_pwd(arg->command, env), 1);
	if (ft_strcmp(arg->command[0], "export") == 0)
		return (ft_export(arg->command, env), 1);
	if (ft_strcmp(arg->command[0], "unset") == 0)
		return (ft_unset(arg->command, env), 1);
	if (ft_strcmp(arg->command[0], "env") == 0)
		return (ft_env(arg->command, env), 1);
	if (ft_strcmp(arg->command[0], "exit") == 0)
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

static void	trim_name_var(char **str)
{
	char	*tmp;
	int		i;

	i = 0;
	while (str[0][i] != '\0')
	{
		if (str[0][i] == '=')
		{
			i++;
			tmp = ft_strdup((char *)(str[0] + i));
			free(str[0]);
			str[0] = tmp;
			return ;
		}
		++i;
	}
}

static char	*explore_path(char *name, char *env_path)
{
	char	**var_path;
	char	*path;
	int		i;
	int		fd;

	i = 0;
	var_path = ft_split(env_path, ':');
	trim_name_var(var_path);
	while (var_path[i] != NULL)
	{
		if (var_path[i][ft_strlen(var_path[i]) - 1] != '/')
			path = ft_strjoin3(var_path[i], "/", name);
		else
			path = ft_strjoin(var_path[i], name);
		if (path == NULL)
		{
			++i;
			continue ;
		}
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
		{
			fd = open(path, O_DIRECTORY);
			if (fd != -1)
				return(close(fd), free_str(var_path), close(fd), NULL);
			return (free_str(var_path), path);
		}
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
	int		fd;

	if (contain_slash(str) != 0)
	{
		if (str[0] != '/')
			path = find_absolute_path(str);
		else
			path = ft_strdup(str);
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) == 0)
			{
				fd = open(path, O_DIRECTORY);
				if (fd != -1)
					return(close(fd), g_error = 127, ft_write_error(NULL, str, "Is a directory"), NULL);
				return (path);
			}
			else
			{
				g_error = 126;
				ft_write_error(NULL, str, strerror(errno));
				return(free(path), NULL);
			}
		}
		g_error = 127;
		ft_write_error(NULL, str, strerror(errno));
		return (free(path), NULL);
	}
	i_path = ft_getenv(env->env, "PATH");
	if (i_path == -1)
	{
		g_error = 127;
		ft_write_error(NULL, str, "command not found");
		return (NULL);
	}
	path = explore_path(str, env->env[i_path]);
	if (path == NULL)
	{
		g_error = 127;
		ft_write_error(NULL, str, "command not found");
	}
	return (path);
}

/**
 * @brief make redirection if fd != 0 + add g_error and write error ??
 * [ATTENTION]Don't have the name of arg_fd so I can't write the error
 * don't know what to do if something failed
 * 
 * @param inst 
 * @param env 
 */
static void	redirection(t_instruction *inst)
{
	if (inst->infile != -2)
	{
		inst->s_infile = dup(STDIN_FILENO);
		if (dup2(inst->infile, STDIN_FILENO) == -1)
		{
			close(inst->infile);
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->infile);
	}
	if (inst->outfile != -2)
	{
		inst->s_outfile = dup(STDOUT_FILENO);
		if (dup2(inst->outfile, STDOUT_FILENO) == -1)
		{
			close(inst->outfile);
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->outfile);
	}
}

static void	reset_redirection(t_instruction *inst)
{
	if (inst->infile != -2)
	{
		if (dup2(inst->s_infile, STDIN_FILENO) == -1)
		{
			g_error = 1;
			close(inst->s_infile);
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->s_infile);
	}
	if (inst->outfile != -2)
	{
		if (dup2(inst->s_outfile, STDOUT_FILENO) == -1)
		{
			g_error = 1;
			close(inst->s_outfile);
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->s_outfile);
	}
}


int	exec(t_instruction *inst, t_env_info *env, int has_ign_sig)
{
	char	*path;
	int		pid;
	int		stat;

	// printf("command\n");
	if (inst == NULL)
		return (-1);
	// printf("command 0\n");
	if (inst->command == NULL)
		return (-1);
	// printf("command 1\n");
	if (inst->command[0] == NULL || *(inst->command[0]) == '\0')
		return (free_str(inst->command), -1);
	// int i = 0;
	// while(inst->command[i] != NULL)
	// {
	// 	printf("inst[%d] = '%s'\n", i, inst->command[i]);
	// 	i++;
	// }
	g_error = 0;
	redirection(inst);
	if (contain_slash(inst->command[0]) == 0 && is_builtins(inst, env) != 0)
		return (reset_redirection(inst), g_error);
	path = find_path_command(inst->command[0], env);
	if (path == NULL)
		return (reset_redirection(inst), free_str(inst->command), g_error);
	pid = fork();
	if (pid == -1)
	{
		g_error = 1;
		return (reset_redirection(inst), free(path), free_str(inst->command), g_error);
	}
	if (pid == 0)
	{
		none_interactive(env->act);
		execve(path, inst->command, env->env);
		return (free(path), free_str(inst->command), g_error);
	}
	if (!has_ign_sig)
		add_error_signals(env->act);
	waitpid(pid, &stat, 0);
	reset_signals(env->act);
	reset_redirection(inst);
	if (WIFEXITED(stat))
	{
		if (WEXITSTATUS(stat) != 0) 
			g_error = WEXITSTATUS(stat);
	}
	free(path);
	free_str(inst->command);
	return (g_error);
}
