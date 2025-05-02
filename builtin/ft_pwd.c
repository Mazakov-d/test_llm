/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazakov <mazakov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:31:50 by mazakov           #+#    #+#             */
/*   Updated: 2025/04/30 15:02:42 by mazakov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd_from_env(t_all *all)
{
	t_env	*ptr;
	int		i;

	ptr = find_in_env(all->env, "PWD");
	if (!ptr)
		return (0);
	i = 0;
	while (ptr->line && ptr->line[i] && ptr->line[i] != '=')
		i++;
	if (ptr->line[i] == '=')
		i++;
	if (!ptr->line[i])
		return (0);
	while (ptr->line && ptr->line[i])
	{
		printf("%c", ptr->line[i]);
		i++;
	}
	printf("\n");
	return (1);
}


int	ft_pwd(t_all *all)
{
	char	*pwd;
	char	*save;
	size_t	size;

	if (pwd_from_env(all))
		return (0);
	pwd = NULL;
	size = 1;
	while (!pwd)
	{
		pwd = malloc(sizeof(char) * size);
		if (!pwd)
			return (1);
		save = pwd;
		pwd = getcwd(pwd, size);
		if (!pwd)
		{
			free(save);
			size++;
		}
		if (size > 60)
			return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}