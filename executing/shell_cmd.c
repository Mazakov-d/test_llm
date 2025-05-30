/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazakov <mazakov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:24:31 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/05/19 22:14:00 by mazakov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_in_child(char **cmds, char **env, char *path_cmd)
{
	execve(path_cmd, cmds, env);
	perror(cmds[0]);
	free_strs(cmds);
	free_strs(env);
	free(path_cmd);
	exit(EXIT_FAILURE);
	return (1);
}

void	clean_resources(char **cmds, char **env, char *path_cmd)
{
	if (cmds)
		free_strs(cmds);
	if (env)
		free_strs(env);
	if (path_cmd)
		free(path_cmd);
}

int	handle_fork_error(char **cmds, char **env, char *path_cmd, t_all *all)
{
	clean_resources(cmds, env, path_cmd);
	put_str_fd("fork : error\n", 2);
	ft_exit(all, NULL);
	return (-1);
}

int	prepare_execution(t_all *all, char ***cmds, char ***env, char **path_cmd)
{
	char	**path;

	path = get_path_env(all->env, all);
	*cmds = cmds_to_strs(all->first->cmds, 1, 0);
	if (!*cmds)
		ft_exit(all, NULL);
	*env = env_to_strs(all->env, 0, 1);
	if (!*env)
	{
		free_strs(*cmds);
		ft_exit(all, NULL);
	}
	*path_cmd = get_path_cmd((*cmds)[0], path, all);
	if (!*path_cmd)
	{
		if (path)
			free_strs(path);
		clean_resources(*cmds, *env, NULL);
		return (0);
	}
	if (path)
		free_strs(path);
	return (1);
}

int	shell_cmd(t_all *all)
{
	char	**cmds;
	char	**env;
	char	*path_cmd;
	int		pid;

	if (!prepare_execution(all, &cmds, &env, &path_cmd))
		return (-1);
	pid = fork();
	if (pid < 0)
		return (handle_fork_error(cmds, env, path_cmd, all));
	if (pid == 0)
	{
		free_all(all);
		execute_in_child(cmds, env, path_cmd);
	}
	clean_resources(cmds, env, path_cmd);
	return (pid);
}
