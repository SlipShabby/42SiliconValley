/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 15:01:18 by ajulanov          #+#    #+#             */
/*   Updated: 2019/06/27 16:04:47 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"
#include "../minilibx/mlx.h"
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

// display help

// void display_help(void)
// {
//     @echo "\n";
//     @echo "|\t\t\tFDF @ 42t\t\t|\n";
// 	@echo "|\t\tmouse left - rotate\t\t\t|\n";
// 	@echo "|\t\tmouse right - move\t\t\t|\n";
// 	@echo "|\t\twheel - zoom in/out\t\t\t|\n";
// 	@echo "|\t\tc - change color\t\t\t|\n";
// 	@echo "|\t\tu - increase z axis\t\t\t|\n";
// 	@echo "|\t\td - decrease z axis\t\t\t|\n";
// }

int		main(int ac, char **av)
{
	int			fd;
	t_map		*map;
	t_coord_val	*coords_stack;
	t_fdf		*fdf;

	errno = 0;
	coords_stack = NULL;
	if (ac == 2)
	{
		if (!((fd = open(argv[1], O_RDONLY)) >= 0))
			kill_bill(ERR_MAP);
		map = map_init();
		if (read_map(fd, &coords_stack, map) == -1)
			kill_bill(ERR_MAP_READING);
		get_map(map, path);
	}
	kill_bill(ERR_USAGE);
	return (0);
}


/* int		main(int argc, char **argv)
{
	int			fd;
	t_map		*map;
	t_coord_val	*coords_stack;
	t_fdf		*fdf;

	errno = 0;
	coords_stack = NULL;
	if (argc == 2)
	{
		if (!((fd = open(argv[1], O_RDONLY)) >= 0))
			terminate(ERR_MAP);
		map = map_init();
		if (read_map(fd, &coords_stack, map) == -1)
			terminate(ERR_MAP_READING);
		fdf = fdf_init(map);
		stack_to_arrays(&coords_stack, map);
		fdf->camera = camera_init(fdf);
		draw(fdf->map, fdf);
		setup_controls(fdf);
		mlx_loop(fdf->mlx);
	}
	terminate(ERR_USAGE);
	return (0);
}
*/
