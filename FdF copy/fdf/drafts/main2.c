/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 16:54:44 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/03 15:10:34 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft/libft.h"
#include "../GNL/get_next_line.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

// ERR STRINGS
# define ERR_USAGE			"usage: ./fdf [filename]"
# define ERR_MAP		    "error: invalid map"
# define ERR_READ	    "error: could not read the map"
# define ERR_CREATE_MAP		"error: could not initialize map"

# define FT_INT_MAX
# define FT_INT_MIN
#define MAP_Z	((*map).map_z)

//________________________________________________________________________


// PROTO for struct

typedef enum
{
	false,
	true
}	t_bool;

typedef struct      s_point
{
	int x;
	int y;
	int color;
}                       t_point;

typedef struct		s_map
{
	void	*win;
	void	*mlx;
	int		height;
	int		width;
	int     **colors;
	int		mouse;
	int		rotate;
	int     fd;
	int	    z_min;
	int		z_max;
	int		z_range;
	int		map_x;
	int		map_y;
	int		**map_z;
} 						t_map;


//PROTO for fn
void				kill_bill(char *s);
t_map   			*create_map(void);
void    			get_map(t_map *map, char *file_name);
void    			check_first_line(t_map *map, char *line, int count_width);
void    			get_map(t_map *map, char *file_name);
void    			get_xy(t_map *map);
void    			get_z(t_map *map);
void 				get_color(t_map *map);
static void			free_strsplit_arr(char **arr);
// int 				ft_atoi_base(const char *str, int base);
t_bool				ft_isnumber(char *str, int base);
// _____________________________________________________________________


int main(int ac, char **av)
{
	t_map   *map;
	int fd;
	
	if (ac == 2)
	{
		if ((fd = open(av[1], O_RDONLY)) < 0)
			kill_bill(ERR_READ);
		printf("file have been read\n");
		map = create_map();
		printf("memory allocated\n");
		// if (map = create_map() == NULL)
		// 		kill_bill(ERR_CREATE_MAP);
		get_map(map, av[1]);
		//     read_map(map);
	}
	else
		kill_bill(ERR_USAGE);
	return (0);
}

// ERR stdout
void	kill_bill(char *s)
{
	if (errno == 0)
		ft_putendl_fd(s, 2);
	else
		perror(s);
	exit(1);
}

//allocate memory 4 data
t_map   *create_map(void)
{
	t_map *map;

	ft_putstr("allocating memory for the data\n");
	if(!(map = (t_map *)ft_memalloc(sizeof(t_map))))
		kill_bill(ERR_CREATE_MAP);
	map->width = 0;
	map->height = 0;
	map->colors = NULL;
	map->coords = NULL;
	// map->z_min = FT_INT_MAX;
	// map->z_max = FT_INT_MIN;
	// map->z_range = 0;
	return (map); 
}


void get_map(t_map *map, char *file_name)
{
	char **line;

	if ((map->fd = open(file_name, O_RDONLY)) < 0)
		kill_bill(ERR_READ);
	get_xy(map);
	close(map->fd);
	if ((map->fd = open(file_name, O_RDONLY)) < 0)
		kill_bill(ERR_READ);
	get_z(map);
	get_color(map);
	// close(map->fd);
}

void get_xy(t_map *map)
{
	char *line;
	int count_width;
	int i;

	while ((get_next_line(map->fd, &line)) == 1)
	{
		i = 0;
		count_width = 0;
		 
		while (line[i])
		{
			if (line[i] == ' ')
				i++;
			else if (line[i] =='\n')
				break ;
			else 
				{
					while (line[i] >= '0' && line[i] <= '9')
					i++;
					count_width++;
				}
		}
	 check_first_line(map, line, count_width);
		map->height++;
		// printf("%d\n", map->height);
		// printf("%d\n", map->width);
		// printf("%d\n", count_width);
	}
}

void check_first_line(t_map *map, char *line, int count_width)
{
	static int first_line = 1;
	
	if (first_line)
		{
			map->width = count_width;
			first_line = 0;
		}
		else if (count_width != map->width)
		{
			kill_bill(ERR_MAP);
		}
}



void get_z(t_map *map)
{
	int i;
	int h;
	char *line;
	char **split;
	
	i = -1;
	h = 0;
	MAP_Z = ft_memalloc(sizeof(int *) * map->height);
	while (++i < map->height)
		MAP_Z[i] = ft_memalloc(sizeof(int) * map->width);
	while ((get_next_line(map->fd, &line)) > 0)
	{
		i = -1;
		split = ft_strsplit(line, ' ');
		while (++i < map->width)
			MAP_Z[h][i] = ft_atoi(split[i]);
		h++;
		while (--i >= 0)
			free(split[i]);
		free(split);
		free(line);
	}
	for(int ii = 0; ii < map->height; ii++)
	{
		for (int nn = 0; nn < map->width; nn++)
			printf("%d ", map->map_z[ii][nn]);
		printf("\n");
	}
}


void get_color(t_map *map)
{
	int i;
	int h;
	char **split;
	char *line;

	i = -1;
	h = 0;
	while (++i < map->height)
		map->colors = ft_memalloc(sizeof(int *) * map->height);
		while ((get_next_line(map->fd, &line)) > 0)
			{
				map->colors[i] = ft_memalloc(sizeof(int) * map->width);
		if (ft_strchr(split[i], ',') != NULL)
				{
					while (*(split[i]) != ',')
						split[i] += 1;
					split[i] += 1;
					map->colors[h][i] = split[1] ? ft_atoi_base(split[i], 16)  : -1;				
				}
				while (--i >= 0)
			free(split[i]);
		free(split);
		free(line);
	}
}


// void get_z(t_map *map)
// {
// 	int i;
// 	int h;
// 	char *line;
// 	char **split;
	
// 	i = -1;
// 	h = 0;
// 	MAP_Z = ft_memalloc(sizeof(int *) * map->height);
// 	map->colors = ft_memalloc(sizeof(int *) * map->height);
// 	while (++i < map->height)
// 		{
// 			MAP_Z[i] = ft_memalloc(sizeof(int) * map->width);
// 		map->colors[i] = ft_memalloc(sizeof(int) * map->width);
// 		}
// 	while ((get_next_line(map->fd, &line)) > 0)
// 	{
// 		i = -1;
// 		split = ft_strsplit(line, ' ');
// 		while (++i < map->width)
// 		{
// 			MAP_Z[h][i] = ft_atoi(split[i]);
// 			//
// 				if (ft_strchr(split[i], ',') == 1)
// 				{
// 					while (*(split[i]) != ',')
// 						split[i] += 1;
// 					split[i] += 1;
// 					map->colors[h][i] = ft_atoi_base();				
// 				}
// 			// 
// 		}
// 		h++;
// 		printf("map_z");
// 		while (--i >= 0)
// 			free(split[i]);
// 		free(split);
// 		free(line);
// 	}
// 	for(int ii = 0; ii < map->height; ii++)
// 	{
// 		for (int nn = 0; nn < map->width; nn++)
// 			printf("%d ", map->map_z[ii][nn]);
// 		printf("\n");
// 	}
// }