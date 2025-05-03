/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazakov <mazakov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:10:31 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/05/03 21:22:10 by mazakov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	extract_exit_status(int wait_status)
{
	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	else if (WIFSIGNALED(wait_status))
		return (128 + WTERMSIG(wait_status));
	return (1);
}

void	wait_for_children(int cmd_count, pid_t *pids, int flag_child, t_all *all)
{
	int	i;
	int	status;
	int	exit_status;

	exit_status = 0;
	i = 0;
	(void)i;
	(void)flag_child;
	(void)exit_status;
	if (pids[cmd_count - 1] != -1) {
		// Wait for the last command first to get its status
		waitpid(pids[cmd_count - 1], &status, 0);
		if (WIFEXITED(status))
			all->status = WEXITSTATUS(status);
		else
			all->status = 0;
		
		// Wait for all remaining child processes
		while (waitpid(-1, NULL, 0) > 0) {
			// Empty loop body - we're just waiting for processes to exit
		}
	}

	// while (i < cmd_count)
	// {
	// 	if (pids[i] != -1)
	// 	{
	// 		waitpid(pids[i], &status, 0);
	// 		if (i == cmd_count - 1)
	// 		{
	// 			if (WIFEXITED(status))
	// 				exit_status = WEXITSTATUS(status);
	// 		}
	// 	}
	// 	i++;
	// }
	// if (flag_child)
	// 	all->status = exit_status;
}

int	*init_pids_array(int cmd_count)
{
	int	*pids;
	int	i;

	pids = malloc(sizeof(int) * cmd_count);
	if (!pids)
		return (NULL);
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = -1;
		i++;
	}
	return (pids);
}
