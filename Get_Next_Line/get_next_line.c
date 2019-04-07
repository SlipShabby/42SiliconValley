/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 19:57:39 by ajulanov          #+#    #+#             */
/*   Updated: 2019/04/06 22:54:20 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int					check(int fd, char **txt, char **line)
{
	int				i;
	char			*tmp;

	i = 0;
	while (txt[fd][i] != '\n' && txt[fd][i] != '\0')
		i++;
	if (txt[fd][i] == '\n')
	{
		*line = ft_strsub(txt[fd], 0, i);
		tmp = ft_strdup(&txt[fd][i + 1]);
		free(txt[fd]);
		txt[fd] = tmp;
	}
	else
	{
		*line = ft_strdup(txt[fd]);
		ft_strdel(&txt[fd]);
	}
	return (1);
}

int					get_next_line(const int fd, char **line)
{
	static char		*txt[MAX_FD];
	char			buff[BUFF_SIZE + 1];
	char			*tmp;
	int				printed;

	if (fd < 0 || fd > MAX_FD || !line || BUFF_SIZE <= 0 || (read(fd, buff, 0)))
		return (-1);
	(!txt[fd] ? txt[fd] = ft_strnew(1) : 0);
	while ((printed = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[printed] = '\0';
		tmp = ft_strjoin(txt[fd], buff);
		ft_strdel(&txt[fd]);
		txt[fd] = tmp;
		if (ft_strchr(buff, '\n'))
			break ;
	}
	if (txt[fd][0] == '\0')
		return (0);
	return (check(fd, txt, line));
}
