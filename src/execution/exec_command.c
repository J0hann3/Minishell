/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:18:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/21 01:44:16 by qthierry         ###   ########.fr       */
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

int	exec(t_instruction *inst, t_env_info *env)
{
	char	*path;
	int		pid;
	int		stat;

	if (inst == NULL || inst->command == NULL)
		return (-1);
	if (inst->command[0] == NULL)
		return (close_fd(inst), free_str(inst->command), -1);
	if (*(inst->command[0]) == '\0')
		return (ft_write_error("", NULL, "command not found"), close_fd(inst), free_str(inst->command), -1);
	if (contain_slash(inst->command[0]) == 0 && is_builtins(inst, env) != 0)
		return (g_error);
	g_error = 0;
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
		free_tree(&(env->tree));
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
