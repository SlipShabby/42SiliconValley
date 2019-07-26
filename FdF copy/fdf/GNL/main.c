/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 20:33:42 by ajulanov          #+#    #+#             */
/*   Updated: 2019/04/05 20:34:35 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int		main(int argc, char **argv)
{
	int fd;
 	int fd2;
	char *line;
	int ret; /* printing gnl return value */
	int i;

	if (argc >= 2)
	{
		i = 0;
		fd = open(argv[1], O_RDONLY);
		while((ret = get_next_line(fd, &line)) == 1)
		{
			i++;
			printf("MAIN:%d, line>%s\n", ret, line);
		}
		close(fd);
		i = 0;
		fd2 = open(argv[2], O_RDONLY);
		while((ret = get_next_line(fd2, &line)) == 1)
		{
			i++;
			printf("MAIN:%d, line>%s\n", ret, line);
		}
		close(fd2);
	}
	return (1);
}
