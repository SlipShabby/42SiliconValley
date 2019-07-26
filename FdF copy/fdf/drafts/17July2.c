/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 19:41:40 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/17 23:04:54 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc -g main.c ../libft/libft.a ../GNL/get_next_line.c -L ../minilibx/ -lmlx -framework OpenGL -framework AppKit

#include "../libft/libft.h"
#include "../GNL/get_next_line.h"
#include "../minilibx/mlx.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// ERR STRINGS
# define ERR_USAGE			"usage: ./fdf [filename]"
# define ERR_MAP		    "error: invalid map"
# define ERR_CONTENT	    "error: invalid content"
# define ERR_READ	    	"error: could not read the map"
# define ERR_CREATE_MAP		"error: could not initialize map"
# define ERR_FDF			"error: FDF initialization"
# define ERR_CAM			"error: camera initialization"



// Initial parameters
# define PRJ_NAME		"42 FDF Jaan"
# define INIT_X			1920
# define INIT_Y			1080
# define MENU_WIDTH		250

# define BACKGROUND			0x222222
# define MENU_BACKGROUND	0x1E1E1E
# define TEXT_COLOR			0xEAEAEA

# define ULONG_MAX		((unsigned long)(~0L))
# define LONG_MAX		((long)(ULONG_MAX >> 1))
# define LONG_MIN		((long)(~LONG_MAX))

# define UINT_MAX		((unsigned)(~0L))
# define INT_MAX		((int)(UINT_MAX >> 1))
# define INT_MINI		((int)(~INT_MAX))

# define CENTER_X		map->width / 2
# define CENTER_Y		map->height / 2

# define M_X			map->move_x
# define M_Y			map->move_y
# define MOOVED_X		((map->move_x - map->mid_x) * map->gap)
# define MOOVED_Y		((map->move_y - map->mid_y) * map->gap)

# define SCALE_Z		10
	

//________________________________________________________________________

// PROTO for struct

typedef enum
{
	false,
	true
}	t_bool;

typedef enum
{
	ISO,
	PARALLEL
}	t_projection;

typedef struct      s_point
{
	int 				x;
	int 				y;
	int 				z;
	int 				color;
	// int 				z_scaled;
}                     		  		t_point;

typedef struct		s_xy
{
	int					x;
	int					y;
}										t_xy;

typedef struct s_bresen
{
	t_xy 				delta;
}									t_bresen;



typedef struct			s_camera
{
	t_projection		projection;
	int					zoom;
	double				alpha;
	double				beta;
	double				gamma;
	float				z_divisor;
	int					x_offset;
	int					y_offset;
}									t_camera;


typedef struct		s_data
{
	void			*img_ptr;		
	char			*img_addr;
	
	int				background;		// background color;
	int				text_color;
	int				base_color;		// color of the maps base;
	int				top_color;		// color of the maps hights;
	unsigned char	back_alpha;	
	unsigned char	pix_alpha;	
}										t_data;


typedef struct		s_map
{
	// mlx init
	void				*win; 
	void				*mlx;

	// parse W & H, check map
	
	int    				fd;
	int					height;
	int					width;

	// store x,y,z

	int					map_x;
	int					map_y;
	t_point				**map_z;
	t_xy				xy;

	
	// draw
	void				*img;
	int					bits_per_pixel;
	int 				scale;
	double				angle_x;
	double				angle_y;
	double				angle_z;

	// to plot lines from point 1 to point 2
	float				x1; 		
	float				y1;
	float				x2;
	float				y2;
	float				z1; 
	float				z2;
	int					dx;
	int					dy;
	int 				d;
	int 				x;
	int 				y;
	int					swap;
	int					delta_x;
	int					delta_y;

	// env settings
	int 				mid_x;		// middle of the screen
	int 				mid_y;
	int 				side;		// 
	int 				gap;		// gap between points
	
	int 				move_x;			// moving x & y  coords
	int 				move_y;

	float 				s; 			// s - slope; s = rise/run
	float 				up;			// y2 - y1
	float 				right;		// x2 - x1
	float				X_res;		// init win X
	float				Y_res;		// init win Y
	int					size_line;
	int					n;			// for new_image

	
	int	    			z_min;		// store z range for color gradient
	int					z_max;

	t_camera			*camera;
	t_data				data;
	t_point				**carte;
	t_bresen			brs;
} 									t_map;


//PROTO for fn
void				kill_bill(char *s);
t_map   			*create_map(void);
void    			get_map(t_map *map, char *file_name);
void    			check_first_line(t_map *map, char *line, int count_width);
void    			get_xy(t_map *map);
void    			get_z(t_map *map);
void 				get_z1(t_map *map, char *line, char **split, int i, int h);
void				get_z2(t_map *map, char **split, int i, int h);
void				get_z3(t_map *map, char *line, int i, int h);
t_bool				ft_isnumber(char *str, int base);
void 				set_env(t_map *map);
void				set_default(t_map *map);
void				draw(t_map *map);
void				background(t_map *map);
void				print_menu(t_map *map);
void				new_image(t_map *map);
void				draw_line(t_map *map);

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
			get_map(map, av[1]);
		set_env(map); 
		mlx_loop(map->mlx);
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
	map->z_max = 0;
	map->z_min = 0;
	return (map);
}

// read the map
void get_map(t_map *map, char *file_name)
{
	if ((map->fd = open(file_name, O_RDONLY)) < 0)
		kill_bill(ERR_READ);
	get_xy(map);
	close(map->fd);
	if ((map->fd = open(file_name, O_RDONLY)) < 0)
		kill_bill(ERR_READ);
	get_z(map);
	close(map->fd);
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
	map->map_z = ft_memalloc(sizeof(t_point *) * map->height);					// MEMORY
	while (++i < map->height)
		map->map_z[i] = ft_memalloc(sizeof(t_point) * map->width);				// MEMORY
		get_z1(map, line, split, i, h);
		// checkpoint
		for(int ii = 0; ii < map->height; ii++)
	{
		for (int nn = 0; nn < map->width; nn++)
			printf("%d,%d ", map->map_z[ii][nn].z, map->map_z[ii][nn].color);
		printf("\n");
	}
	printf("checkpoint: get_z + color\n");
	printf("z max : %d\n", map->z_max);
	printf("z min : %d\n", map->z_min);
}


void get_z1(t_map *map, char *line, char **split, int i, int h)
{
	while ((get_next_line(map->fd, &line)) > 0)
	{
		i = -1;
		split = ft_strsplit(line, ' ');											// MEMORY
		while (++i < map->width) 
		{
			map->map_z[h][i].x = i;
			map->map_z[h][i].y = h;
			get_z2(map, split, i, h);
			map->z_max = map->map_z[h][i].z > map->z_max ? map->map_z[h][i].z : map->z_max;
			map->z_min = map->map_z[h][i].z < map->z_min ? map->map_z[h][i].z : map->z_min;
			i++;
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


//________________________________________________________________________
//
// DRAW a MAP
// _______________________________________________________________________


void set_env(t_map *map)
{
	set_default(map);
	// if (!(map = (t_map *)ft_memalloc(sizeof(t_map))))
	// 	kill_bill(ERR_FDF);
	draw(map);
}


// set initial params for the screen and map transformation (gap)

void set_default(t_map *map)
{
	map->x1 = 0; 
	map->y1 = 0;
	map->x2 = 0; 
	map->y2 = 0;
	map->side = map->width > map->height ? map->width : map->height;
	map->gap = map->width > map->height ? 700 / (map->width) : 700 /(map->height);
	map->s = 0;
	map->up = 0;
	map->right = 0;
	map->X_res = 800 + map->gap * map->side;
	map->Y_res = map->X_res - 300;

	printf("Dimension = %f x %f\nPixel gap = %d\n", map->X_res, map->Y_res, map->gap);

	map->move_x = 0;
	map->move_y = 0;
	map->mlx = mlx_init();
    map->win = mlx_new_window(map->mlx, map->X_res,map->Y_res, PRJ_NAME);
	print_menu(map);
}

void	print_menu(t_map *map)
{
	int		y;
	void	*mlx;
	void	*win;

	y = 0;
	mlx = map->mlx;
	win = map->win;
	mlx_string_put(mlx, win, 65, y += 20, TEXT_COLOR, "How to Use");
	mlx_string_put(mlx, win, 15, y += 35, TEXT_COLOR, "Zoom: Scroll or +/-");
	mlx_string_put(mlx, win, 15, y += 30, TEXT_COLOR, "Move: Arrows");
	mlx_string_put(mlx, win, 15, y += 30, TEXT_COLOR, "Flatten: </>");
	mlx_string_put(mlx, win, 15, y += 30, TEXT_COLOR, "Rotate: Press & Move");
	mlx_string_put(mlx, win, 15, y += 30, TEXT_COLOR, "Rotate:");
	mlx_string_put(mlx, win, 57, y += 25, TEXT_COLOR, "X-Axis - 2/8");
	mlx_string_put(mlx, win, 57, y += 25, TEXT_COLOR, "Y-Axis - 4/6");
	mlx_string_put(mlx, win, 57, y += 25, TEXT_COLOR, "Z-Axis - 1(3)/7(9)");
	mlx_string_put(mlx, win, 15, y += 30, TEXT_COLOR, "Projection");
	mlx_string_put(mlx, win, 57, y += 25, TEXT_COLOR, "ISO: I Key");
	mlx_string_put(mlx, win, 57, y += 25, TEXT_COLOR, "Parallel: P Key");
}

void draw(t_map *map)
{
	draw_line(map);			// line from point 1 to point 2 (bresenheim)
							// from 0 to any point in positive axis
							// from 0 to any point in negative axis
							// https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html

	// positive axis
	// background(map);
	// new_image(map);
	// redraw_image(map);
}


// void draw_line(t_map *map)
// {
// 	float slope;
// 	float err;
// 	int y;
// 	int x;

// 	slope = ((float)map->y2 - map->y1) / ((float) map->x2 - map->x1);
// 	float err = 0.0;
// 	x = map->x1;
// 	while (x <= map->x1)
// 	{
// 		mlx_pixel_put(map->mlx, map->win, x, y, 0x0FF0FF);
// 		err += slope;
// 		if (err >= 0.5)
// 		{
// 			y++;
// 			err = -1.0;
// 		}
// 	}
// 	x++;
// }


// HOW TO DRAW A LINE

/* 

Digital Differential Analyzer (DDA) algorithm

~ step 1
we need the input of 4 coords: x0, x1, y0, y1

~ step 2
get the delta 
dx = x1 - x0
dy = y1 - y0

~ step 3
Based on the calculated difference in step-2, 
you need to identify the number of steps to put pixel. 
If dx > dy, then you need more steps in x coordinate; 
otherwise in y coordinate.

if (absolute(dx) > absolute(dy))
   Steps = absolute(dx);
else
   Steps = absolute(dy);


~step 4 
calculate the increment in x or y coord

Xincrement = dx / (float) steps;
Yincrement = dy / (float) steps;

__________________________________
Bresenham’s Line Generation 
__________________________________

y = m(x0  + 1) + b

d_lower = y1 - y0 = m(x0 + 1) + b - y0

d_upper = (y0 + 1) - y1 = y0 + 1 - m(x0 +1) - b

d_lower - d_upper = 2dy.x0 - 2dx.y0 + C

p0 = 2dy - dx


A. if slope m < 1
~ step 1 
	input: x0, y0
~ step 2 
	plot (x0, y0)
~ step 3 
	calculate: dx, dy, 2 dy, 2dy - dx, p0

	for each x0: 
		if p < 0 
			p0 + 1 = p0 + 2dy
		else
			(x0, y+1)
			p0 + 1 = p0 + 2dy - 2dx

~ step 4 
	repeat step 4 (dx -1) times
 */


void draw_line(t_map *map)
{	
	int i;
	
	i = 0;
	map->x1 = 0;
	map->x2 = 1100;
	map->y1 = 0;
	map->y2 = 1000;
	map->x = map->x1;
	map->y = map->y1;
	map->dx = fabs(map->x2 - map->x1);
	map->dy = fabs(map->y2 - map->y1);
	map->delta_x = (map->x2 - map->x1) ? 1 : -1;
	map->delta_y = (map->y2 - map->y1) ? 1 : -1;
	map->swap = 0;
	
	if (map->dy > map->dx)
	{
		ft_swap(&map->dx, &map->dy);
		map->swap = 1;
	}
	map->d = 2 * map->dy - map->dx;
	while (i++ < map->dx)
	{
		mlx_pixel_put(map->mlx, map->win, map->x, map->y, 0x003366FF);
		while (map->d >= 0)
		{
			map->d = map->d - 2 * map->dx;
		}
		map->d = map->d + 2 * map->dy;
		if (map->swap)
		{
				map->x += map->delta_x;
				printf("map->brs.delta.x: %d\n", map->delta_x);
		}
		else {
				map->y += map->delta_y;
				printf("map->brs.delta.y: %d\n", map->delta_y);
		}
	}
}
 