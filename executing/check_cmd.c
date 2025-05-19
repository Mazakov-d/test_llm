/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazakov <mazakov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:52:41 by mazakov           #+#    #+#             */
/*   Updated: 2025/05/19 23:59:28 by mazakov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    ft_strchr(const char *s, int c)
{
    int    i;

    i = 0;
    if (!s)
        return (0);
        
    while (s[i])
    {
        if (s[i] == (char)c)
            return (1);
        i++;
    }
    return (0);
}

char	*handle_not_found(char *cmd, t_all *all)
{
	if (ft_strcmp(cmd, "/") == 2)
		put_str_error(cmd, "No such file or directory", 2);
	else
		put_str_error(cmd, "command not found", 2);
	
	all->status = 127;
	return (NULL);
}

int	index_path_cmd(char *cmd, char **path)
{
	int		i;
	char	*path_cmd;

	i = 0;
	if (!cmd || !path || ft_strcmp(cmd, "/") == 2)
		return (-1);
		
	while (path[i])
	{
		path_cmd = ft_strcat(path[i], cmd, 0, 0);
		if (!path_cmd)
			return (-1);
		if (access(path_cmd, F_OK | X_OK) == 0)
		{
			free(path_cmd);
			return (i);
		}
		free(path_cmd);
		i++;
	}
	return (-1);
}

int    check_local(char *cmd, char **path_cmd)
{
	char		*pwd;
	char		*tmp;
	struct stat	statbuf;

	pwd = get_pwd(NULL);
	if (!pwd)
	{
		*path_cmd = NULL;
		return (0);
	}
	tmp = ft_strcat(pwd, "/", 0, 0);
	free(pwd);
	if (!tmp)
		return (0);
	*path_cmd = ft_strcat(tmp, cmd, 0, 0);
	free(tmp);
	if (!*path_cmd)
		return (0);
	if (access(*path_cmd, F_OK) == 0)
	{
		if (lstat(*path_cmd, &statbuf) == 0 && !S_ISDIR(statbuf.st_mode))
			return (1);
	}
	free(*path_cmd);
	*path_cmd = NULL;
	return (0);
}
/* From check_cmd.c */

char    *check_file_permission(char *cmd, char *path_cmd, t_all *all)
{
    struct stat    statbuf;

    if (lstat(path_cmd, &statbuf) == 0)
    {
        if (S_ISDIR(statbuf.st_mode))
        {
            put_str_error(cmd, "Is a directory", 2);
            all->status = 126;
            free(path_cmd);
            return (NULL);
        }
        
        if (!S_ISREG(statbuf.st_mode))
        {
            free(path_cmd);
            return (NULL);
        }
        
        if (access(path_cmd, X_OK) != 0)
        {
            put_str_error(cmd, "Permission denied", 2);
            all->status = 126;
            free(path_cmd);
            return (NULL);
        }
        
        return (path_cmd);
    }
    
    free(path_cmd);
    return (NULL);
}

char    *get_path_cmd(char *cmd, char **path, t_all *all)
{
    int            i;
    char        *path_cmd;
    struct stat    statbuf;

    if (!cmd || cmd[0] == '\0')
        return (NULL);
    
    /* If command includes path indicators like ./ or / */
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) != 0)
        {
            put_str_error(cmd, "No such file or directory", 2);
            all->status = 127;
            return (NULL);
        }
        
        if (lstat(cmd, &statbuf) == 0)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                put_str_error(cmd, "Is a directory", 2);
                all->status = 126;
                return (NULL);
            }
        }
        
        if (access(cmd, X_OK) != 0)
        {
            put_str_error(cmd, "Permission denied", 2);
            all->status = 126;
            return (NULL);
        }
        
        return (ft_strdup(cmd));
    }
    
    /* Command has no path indicators, search in PATH */
    if (!path)
    {
        put_str_error(cmd, "command not found", 2);
        all->status = 127;
        return (NULL);
    }
    
    i = 0;
    while (path[i])
    {
        path_cmd = ft_strcat(path[i], cmd, 0, 0);
        if (!path_cmd)
            return (NULL);
        
        if (access(path_cmd, F_OK) == 0)
        {
            if (lstat(path_cmd, &statbuf) == 0)
            {
                if (S_ISDIR(statbuf.st_mode))
                {
                    free(path_cmd);
                    i++;
                    continue;
                }
                
                if (access(path_cmd, X_OK) == 0)
                    return (path_cmd);
                else
                {
                    free(path_cmd);
                    i++;
                    continue;
                }
            }
        }
        
        free(path_cmd);
        i++;
    }
    
    put_str_error(cmd, "command not found", 2);
    all->status = 127;
    return (NULL);
}

/* New helper function */


