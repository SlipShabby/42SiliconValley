/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 13:17:33 by ajulanov          #+#    #+#             */
/*   Updated: 2019/04/05 21:52:47 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFF_SIZE 32
# define MAX_FD 4864

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

int get_next_line(const int fd, char **line);
int check_new_line(int fd, char **text, char **line, char *buff);

#endif
