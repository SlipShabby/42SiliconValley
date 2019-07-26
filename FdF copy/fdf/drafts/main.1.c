/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 15:01:18 by ajulanov          #+#    #+#             */
/*   Updated: 2019/06/09 23:53:43 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"
#include "../minilibx/mlx.h"
#include <unistd.h>

// display help

void display_help(void)
{
    @echo "\n";
    @echo "|\t\t\tFDF @ 42\t\t\t|\n";
	@echo "|\t\tmouse left - rotate\t\t\t|\n";
	@echo "|\t\tmouse right - move\t\t\t|\n";
	@echo "|\t\twheel - zoom in/out\t\t\t|\n";
	@echo "|\t\tc - change color\t\t\t|\n";
	@echo "|\t\tu - increase z axis\t\t\t|\n";
	@echo "|\t\td - decrease z axis\t\t\t|\n";
}

void	kill_bill(char *s)
{
	if (errno == 0)
		ft_putendl_fd(s, 2);
	else
		perror(s);
	exit(1);
}

// parsing file

int main(int ac, char **av)
{
int fd;
t_map *3d_map;
t_mlx *mlx;

errno = 0;
xyz = NULL;
if (ac == 2)
{
if (!((fd = open(av[1], O_RDONLY)) >= 0))
	kill_bill(ERR_MAP);
3d_map = map_init();

// if (read_3d_map(fd, &xyz, 3d_map) == -1)
	 kill_bill(ERR_MAP_READING);
fdf = fdf_init(3d_map);
if (fdf == NULL)
	kill_bill(ERR_MAP_INIT);

fdf -> camera = camera_init(fdf);
draw(fdf->map, fdf);
setup_hooks(fdf);
mlx_loop(fdf->mlx);
}
kill_bill(ERR_USAGE);
return (0);
}