/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_space.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:10:16 by yassinefahf       #+#    #+#             */
/*   Updated: 2025/05/05 14:21:28 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redir(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

static int	irs(char c)
{
	if (is_redir(c) || c == ' ')
		return (1);
	return (0);
}

int	count_space(char *line)
{
	int	count;
	int	i;
	int	tmp;
	int	sq;
	int	dq;

	tmp = 0;
	i = 0;
	sq = 0;
	dq = 0;
	while (line && line[i])
	{
		is_in_quote(line[i], &sq, &dq);
		if (is_redir(line[i]))
		{
			if (line[i + 1] && !irs(line[i + 1]) && !(sq % 2) && !(dq % 2))
				tmp++;
			if (i > 0 && line[i - 1] && !irs(line[i - 1])
				&& !(sq % 2) && !(dq % 2))
				tmp++;
		}
		i++;
	}
	count = i + tmp;
	return (count);
}

static int	only_redir(char b, char c)
{
	if (c == '<' && b != '<' && b != ' ')
		return (1);
	if (c == '>' && b != '>' && b != ' ')
		return (1);
	if (b == '<' && c != '<' && c != ' ')
		return (1);
	if (b == '>' && c != '>' && c != ' ')
		return (1);
	return (0);
}

char	*add_space(char *line)
{
	char	*dest;
	int		i;
	int		tmp;
	int		sq;
	int		dq;

	i = 0;
	tmp = 0;
	sq = 0;
	dq = 0;
	dest = ft_calloc(count_space(line) + 1, sizeof(char));
	if (!dest)
		return (NULL);
	while (line && line[i])
	{
		is_in_quote(line[i], &sq, &dq);
		dest[tmp] = line[i];
		if (line[i + 1] && only_redir(line[i], line[i + 1])
			&& !(sq % 2) && !(dq % 2))
			dest[++tmp] = ' ';
		i++;
		tmp++;
	}
	dest[tmp] = '\0';
	return (dest);
}
