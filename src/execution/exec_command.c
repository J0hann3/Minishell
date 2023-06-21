/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:18:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/21 14:49:05 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	contain_slash(char *str);

static void	ft_builtins(void (function)(char **, t_env_info *),
	t_instruction *arg, t_env_info *env)
{
	redirection(arg);
	function(arg->command, env);
	reset_redirection(arg);
}

static int	is_builtins(t_instruction *arg, t_env_info *env)
{
	if (ft_strcmp(arg->command[0], "exit") == 0)
	{
		redirection(arg);
		ft_exit(arg, env);
		reset_redirection(arg);
		return (1);
	}
	g_error = 0;
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
	return (0);
}

static void	ft_waitpid(t_env_info *env, t_instruction *inst, int pid,
	char *path)
{
	int		stat;

	ign_signals(env->act);
	waitpid(pid, &stat, 0);
	reset_signals(env->act);
	if (WIFSIGNALED(stat))
		g_error = 128 + WTERMSIG(stat);
	else
		g_error = WEXITSTATUS(stat);
	free(path);
	free_instructions(inst);
}

static bool	is_null( t_instruction *inst)
{
	if (inst == NULL || inst->command == NULL)
		return (free_instructions(inst), true);
	if (inst->command[0] == NULL)
		return (free_instructions(inst), true);
	if (*(inst->command[0]) == '\0')
		return (g_error = 127, ft_write_error("", NULL, "command not found"),
			free_instructions(inst), true);
	return (false);
}

int	exec(t_instruction *inst, t_env_info *env)
{
	char	*path;
	int		pid;

	if (is_null(inst))
		return (g_error);
	if (!contain_slash(inst->command[0]) && is_builtins(inst, env) != 0)
		return (close_fd(inst), free(inst), g_error);
	g_error = 0;
	path = find_path_command(inst->command[0], env);
	if (path == NULL)
		return (free_instructions(inst), g_error);
	pid = fork();
	if (pid == -1)
		return (g_error = 1, free_instructions(inst), free(path),
			ft_write_error("fork", NULL, strerror(errno)), g_error);
	if (pid == 0)
	{
		get_next_line(-1);
		redirection_fork(inst);
		none_interactive(env->act);
		free_tree(&(env->tree));
		execve(path, inst->command, env->env);
	}
	ft_waitpid(env, inst, pid, path);
	return (g_error);
}
