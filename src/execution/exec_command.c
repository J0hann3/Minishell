/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:18:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/03 11:58:14 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_builtins(void(function)(char **, t_env_info *), t_instruction *arg, t_env_info *env)
{
	redirection(arg);
	function(arg->command, env);
	reset_redirection(arg);
}

static int	is_builtins(t_instruction *arg, t_env_info *env)
{
	if (ft_strcmp(arg->command[0], "echo") == 0)
		return (ft_builtins(&ft_echo, arg, env), 1);
	if (ft_strcmp(arg->command[0], "cd") == 0)
		return (ft_builtins(&ft_cd, arg, env), 1);
	if (ft_strcmp(arg->command[0], "pwd") == 0)
		return (ft_builtins(&ft_pwd, arg, env), 1);
	if (ft_strcmp(arg->command[0], "export") == 0)
		return (ft_builtins(&ft_export, arg, env), 1);
	if (ft_strcmp(arg->command[0], "unset") == 0)
		return (ft_builtins(&ft_unset, arg, env), 1);
	if (ft_strcmp(arg->command[0], "env") == 0)
		return (ft_builtins(&ft_env, arg, env), 1);
	if (ft_strcmp(arg->command[0], "exit") == 0)
	{
		redirection(arg);
		ft_exit(arg, env);			// double free detected
		reset_redirection(arg);
		return (1);
	}
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
	if (str[0] == NULL)
		return ;
	while (str[0][i] != '\0')
	{
		if (str[0][i] == '=')
		{
			i++;
			tmp = ft_strdup((char *)(str[0] + i));
			if (tmp == NULL)
				return (g_error = 2, ft_write_error(NULL, NULL, strerror((errno))));
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
	if (var_path == NULL)
		return (NULL);
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
		if (path == NULL)
			return (g_error = 2, ft_write_error(NULL, NULL, strerror((errno))), NULL);
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
void	redirection(t_instruction *inst)
{
	if (inst->infile >= 0)
	{
		inst->s_infile = dup(STDIN_FILENO);
		if (inst->s_infile == -1)
			return (g_error = 1, ft_write_error("file", NULL, strerror((errno))));
		if (dup2(inst->infile, STDIN_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->infile);
		inst->infile = -2;
	}
	if (inst->outfile >= 0)
	{
		inst->s_outfile = dup(STDOUT_FILENO);
		if (inst->s_outfile == -1)
			return (g_error = 1, ft_write_error("file", NULL, strerror((errno))));
		if (dup2(inst->outfile, STDOUT_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->outfile);
		inst->outfile = -2;
	}
}

void	redirection_fork(t_instruction *inst)
{
	if (inst->infile >= 0)
	{
		if (dup2(inst->infile, STDIN_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->infile);
		inst->infile = -2;
	}
	if (inst->outfile >= 0)
	{
		if (dup2(inst->outfile, STDOUT_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->outfile);
		inst->outfile = -2;
	}
}

void	reset_redirection(t_instruction *inst)
{
	if (inst->infile >= 0)
	{
		if (dup2(inst->s_infile, STDIN_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->s_infile);
		inst->s_infile = -2;
	}
	if (inst->outfile >= 0)
	{
		if (dup2(inst->s_outfile, STDOUT_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->s_outfile);
		inst->s_outfile = -2;
	}
}


int	exec(t_instruction *inst, t_env_info *env)
{
	char	*path;
	int		pid;
	int		stat;

	if (inst == NULL || inst->command == NULL)
		return (-1);
	if (inst->command[0] == NULL || *(inst->command[0]) == '\0')
		return (close_fd(inst), free_str(inst->command), -1);
	g_error = 0;
	if (contain_slash(inst->command[0]) == 0 && is_builtins(inst, env) != 0)
		return (g_error);
	path = find_path_command(inst->command[0], env);
	if (path == NULL)
		return (close_fd(inst), free_str(inst->command), g_error);
	pid = fork();
	if (pid == -1)
	{
		g_error = 1;
		ft_write_error("fork", NULL, strerror(errno));
		return (close_fd(inst), free(path), free_str(inst->command), g_error);
	}
	if (pid == 0)
	{
		redirection_fork(inst);
		none_interactive(env->act);
		execve(path, inst->command, env->env);
		return (free(path), free_str(inst->command), g_error);
	}
	ign_signals(env->act);
	close_fd(inst);
	if (waitpid(pid, &stat, 0) == -1)
		ft_write_error("fork", NULL, strerror(errno));
	reset_signals(env->act);
	if (WIFSIGNALED(stat))
		g_error = 128 + WTERMSIG(stat);
	else
		g_error = WEXITSTATUS(stat);
	free(path);
	free_str(inst->command);
	return (g_error);
}
