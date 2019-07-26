/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 19:41:40 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/05 16:23:15 by ajulanov         ###   ########.fr       */
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
	int z;
	int color;
}                       t_point;

typedef struct		s_map
{
	// void	*win; 
	// void	*mlx;
	int		height;
	int		width;
	// int     **colors;
	// int		mouse;
	// int		rotate;
	int     fd;
	// int	    z_min;
	// int		z_max;
	// int		z_range;
	int		map_x;
	int		map_y;
	t_point		**map_z;
} 						t_map;


//PROTO for fn
void				kill_bill(char *s);
t_map   			*create_map(void);
void    			get_map(t_map *map, char *file_name);
void    			check_first_line(t_map *map, char *line, int count_width);
void    			get_xy(t_map *map);
void    			get_z(t_map *map);
void 				get_z1(t_map *map);
void				get_z2(t_map *map, char **split, int i, int h);
// void 				get_color(t_map *map);
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
		printf("file has been read\n");
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
	map->map_z = NULL;
	return (map);
}

// read the map
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
	// close(map->fd);
}


// get xy
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
					while (line[i] != '\0' && line[i] != ' ')
					i++;
					count_width++;
				}
		}
	 check_first_line(map, line, count_width);
		map->height++;
	}
	printf("map height:  %d\n", map->height);
	printf("map width:   %d\n", map->width);
	printf("count width: %d\n", count_width);
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
	char *line;
	char **split;
	
	i = -1;
	map->map_z = ft_memalloc(sizeof(t_point *) * map->height);					// MEMORY
	while (++i < map->height)
		map->map_z[i] = ft_memalloc(sizeof(t_point) * map->width);				// MEMORY
		get_z1(map);
}


void get_z1(t_map *map)
{
	int i;
	int h;
	char *line;
	char **split;

	i = -1;
	h = 0;	
	while ((get_next_line(map->fd, &line)) > 0)
	{
		i = -1;
		split = ft_strsplit(line, ' ');											// MEMORY
		while (++i < map->width) 
		{
			map->map_z[h][i].x = i;
			map->map_z[h][i].y = h;
			get_z2(map, split, i, h);
		}
		h++;
		while (--i >= 0)
			free(split[i]); 
		free(split);															//FREE
		free(line);		
	}
}

void get_z2(t_map *map, char **split, int i, int h)
{
	char **tmp;
	
			if (ft_strchr(split[i], ',') != NULL) 
			{
				tmp = ft_strsplit(split[i], ',');								// MEMORY
				map->map_z[h][i].z = ft_atoi(tmp[0]);
				map->map_z[h][i].color = ft_atoi_base(tmp[1]+2, 16);
				free(tmp[0]);													//FREE
				free(tmp[1]);													//FREE
			}
			else 
			{
				map->map_z[h][i].z = ft_atoi(split[i]);    
				map->map_z[h][i].color = 0xF31423;
			}
}

// void get_z(t_map *map)
// {
// 	int i;
// 	int h;
// 	char *line;
// 	char **tmp;
// 	char **split;
	
// 	i = -1;
// 	h = 0;
// 	// allocate memory for the y coords
// 	map->map_z = ft_memalloc(sizeof(t_point *) * map->height);					// MEMORY
// 	// allocate memory for the x coords
// 	while (++i < map->height)
// 		map->map_z[i] = ft_memalloc(sizeof(t_point) * map->width);				// MEMORY
// 	// loop to read the x,y,z, colors

// 	while ((get_next_line(map->fd, &line)) > 0)
// 	{
// 		i = -1;
// 		// split by space, get the coords
// 		split = ft_strsplit(line, ' ');											// MEMORY
// 		while (++i < map->width) 
// 		{
// 			// **map_z in structure -> [][] to create the coords
// 			map->map_z[h][i].x = i;
// 			map->map_z[h][i].y = h;
// 			// split by comma
// 			if (ft_strchr(split[i], ',') != NULL) 
// 			{
// 				tmp = ft_strsplit(split[i], ',');								// MEMORY
// 				map->map_z[h][i].z = ft_atoi(tmp[0]);
// 				map->map_z[h][i].color = ft_atoi_base(tmp[1]+2, 16);
// 				free(tmp[0]);													//FREE
// 				free(tmp[1]);													//FREE
// 			}
// 			// store split[i] without the color with predefined color
// 			else 
// 			{
// 				map->map_z[h][i].z = ft_atoi(split[i]);    
// 				map->map_z[h][i].color = 0xF31423;
// 			}
// 		}
// 		h++;
// 		// free everything
// 		while (--i >= 0)
// 			free(split[i]); 													//FREE
// 		free(split);															//FREE
// 		free(line);																//FREE
// 	}
// }