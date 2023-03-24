/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:18:41 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/24 14:59:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtins(char **arg, t_env_info *env)
{
	if (ft_strcmp(arg[0], "echo") == 0)
		return (ft_echo(arg, (const t_env_info *)env), 1);
	if (ft_strcmp(arg[0], "cd") == 0)
		return (ft_cd(arg, env), 1);
	if (ft_strcmp(arg[0], "pwd") == 0)
		return (ft_pwd(arg, env), 1);
	if (ft_strcmp(arg[0], "export") == 0)
		return (ft_export(arg, env), 1);
	if (ft_strcmp(arg[0], "unset") == 0)
		return (ft_unset(arg, env), 1);
	if (ft_strcmp(arg[0], "env") == 0)
		return (ft_env(arg, env), 1);
	if (ft_strcmp(arg[0], "exit") == 0)
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
			continue ;	//code error
		}
		// printf("TEST : %s\n", path);
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
		// printf("PATH TEST : %s\n", path);
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)		//not sure condition F_OX is usefull ?
			return (path);
		env->error = 127;
		ft_write_error(NULL, str, strerror(errno));
		return (free(path), NULL);
	}
	i_path = ft_getenv(env->env, "PATH");
	if (i_path == -1)
	{
		env->error = 1;			//code error au pif
		return (NULL);
	}
	path = explore_path(str, env->env[i_path]);
	if (path == NULL)
	{
		env->error = 127;
		ft_write_error(NULL, str, "command not found");
	}
	return (path);
}

/**
 * @brief make redirection before execution + error
 * 
 * [ATTENTION]	DON'T HAVE NAME OF FILE TO PRINT ERROR MESSSAGES
 * 
 * @param inst 
 * @param env 
 */
void	redirection(t_instruction *inst, t_env_info *env)
{
	if (inst->infile != 0)
		if (dup2(inst->infile, 0) != 0)
		{
			env->error = 1;
		}
	if (inst->outfile != 0)
		if (dup2(inst->outfile, 1) != 0)
		{
			env->error = 1;
		}
	if (inst->outerror != 0)
		if (dup2(inst->outerror, 2) != 0)
		{
			env->error = 1;
		}
}
/**
 * @brief create and fork for execution
 * child (pid==0)-> write permission
 * parent (pid > 0) -> read permission and wait
 * 
 * @param env 	write erroor in env if necessary
 * @param operand	create pipe if operand == e_pipe
 * @return int : pid of process on success, else -1
 */
int	ft_pipe(t_env_info *env, enum e_meta_character operand)
{
	int	fildes[2];
	int	pid;
	
	if (operand == e_pipe)
	{
		if (pipe(fildes) != 0)
		{
			env->error = 1;
			return (-1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		env->error = 1;
		return (-1);
	}
	if (pid == 0)
	{
		close(fildes[0]);
		dup2(fildes[1], 1);
		return (pid);
	}
	else
	{
		close(fildes[1]);
		dup2(fildes[0], 0);
		return (pid);
	}
}

void	exec(t_instruction *inst, t_env_info *env)
{
	char	*path;
	int		pid;
	int		stat;

	// reset error before new commands
	env->error = 0;
	//Check command
	if (inst == NULL)
		return ;
	if (inst->command == NULL)
		return ;
	if (inst->command[0] == NULL)
		return ;

	//Redirection
	redirection(inst, env);
	
	//	Find Path
	if (contain_slash(inst->command[0]) == 0 && is_builtins(inst->command, env) != 0)
		return ;
	path = find_path_command(inst->command[0], env);
	if (path == NULL)
		return ;
	
	//exec in fork or no ??
	pid = fork();
	if (pid == -1)
	{
		env->error = 1;
		return ;
	}
	if (pid == 0)
	{
		execve(path, inst->command, env->env);
		return ;
	}

	//recup exit of function
	waitpid(pid, &stat, 0);
	free(path);
	free_str(inst->command);
	if (WIFEXITED(stat))
	{
		env->error = WEXITSTATUS(stat);
	}
}