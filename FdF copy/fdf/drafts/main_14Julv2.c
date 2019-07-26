/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 19:41:40 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/14 14:02:41 by ajulanov         ###   ########.fr       */
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
# define PRJ_NAME		"42 FDF"
# define INIT_X			1920
# define INIT_Y			1080
# define MENU_WIDTH		250

# define BACKGROUND			0x222222
# define MENU_BACKGROUND	0x1E1E1E

# define FT_MIN			(A, B) (((A) < (B)) ? (A) : (B))
# define FT_MAX			(A, B) (((A) > (B)) ? (A) : (B))

# define FT_ABS			(X) (((X) < 0) ? (-(X)) : (X))

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

# define SCALE_Z		7
	

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

	
	// draw
	void				*img;
	int					bits_per_pixel;
	int 				scale;
	char				*data;
	double				angle_x;
	double				angle_y;
	double				angle_z;

	// env settings

	float				x1; 		// to plot lines from point 1 to point 2
	float				y1;
	float				x2;
	float				y2;
	float				z1; 
	float				z2;

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

	
	int	    			z_min;		// store z range for color gradient
	int					z_max;

	t_camera			*camera;
	t_point				**carte;
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
void				draw_map(t_map *map);
t_point 			project(t_point c, t_map *map);
static void			rotate_x(int *y, int *z, double alpha);
static void			rotate_y(int *x, int *z, double beta);
static void			rotate_z(int *x, int *y, double gamma);
static void			iso(int *x, int *y, int z);


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
		// draw_map(map);
		// mlx_loop(map->mlx);
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
	if (!(map = (t_map *)ft_memalloc(sizeof(t_map))))
		kill_bill(ERR_FDF);
	draw(map);
	// draw_map(map, map->hooks);
	
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
	map->carte = make_carte(map);
	printf("make_carte: done \n");
	map->mlx = mlx_init();
    map->win = mlx_new_window(map->mlx, map->X_res, map->Y_res, PRJ_NAME);
}

void draw(t_map *map)
{
	int x;
	int y;

	background(map);
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (x != map->width - 1)
				draw_line(project(new_point(x, y, map), map), project(new_point(x + 1 , y, map), map), map);
			if (y != map->height - 1)
				draw_line(project(new_point(x, y, map), map), project(new_point(x, y + 1, map), map), map);
		x++;
		}
	y++;	
	}
	mlx_put_image_to_window(map->mlx, map->win, map->img, 0, 0);
	print_menu(map);
}

void background(t_map *map)
{
	int *image;
	int i;

	ft_bzero(map->data, map->X_res * map->Y_res * (map->bits_per_pixel / 7));
	image = (int *)(map->data);
	i = 0;
	while (i < map->Y_res * map->X_res)
	{
		image[i] = (i * map->X_res < MENU_WIDTH) ? MENU_BACKGROUND : BACKGROUND;
		i++;
	}
}

void draw_line(t_point a, t_point b, t_map *map)
{
	t_point delta;
	t_point sign;
	t_point recent;
	int error[2];

	delta.x = FT_ABS(b.x - a.x);
	delta.y = FT_ABS(b.y - a.y);
	sign.x = a.x < b.x ? 1: -1;
	sign.y = a.y < b.y ? 1: -1;
	error[0] = delta.x - delta.y;
	recent = a;
	while (recent.x != b.x || recent.x != b.y)
	{
		mlx_put_pixel(map, recent.x, recent.y, get_color(recent, a, b, delta));
		if ((error[1] = error[0] * 2) > -delta.y)
		{
			error[0]-= delta.y;
			recent.x += sign.x;
		}
		if (error[1] < delta.x)
		{
			error[0] += delta.x;
			recent.y += sign.y;
		}
	}
}

void put_pixel(t_map *map, int x, int y, int color)
{
	int i;

	if (x > MENU_WIDTH && x < map->X_res && y > 0 && y < map-> Y_res)
	{
		i = (x * map->bits_per_pixel /7) + (y * map->size_line);
		map->data[i] = color;
		map->data[++i] = color >> 8;
		map->data[++i] = color >> 16;
	}
}

t_point project(t_point c, t_map *map)
{
	c.x *= map->camera->zoom;
	c.y *= map->camera->zoom;
	c.z *= map->camera->zoom / map->camera->z_divisor;
	c.x -= (map->width * map->camera->zoom) / 2;
	c.y -= (map->height * map->camera->zoom) / 2;
	rotate_x(&c.y, &c.z, map->camera->alpha);
	rotate_y(&c.x, &c.z, map->camera->beta);
	rotate_z(&c.x, &c.y, map->camera->gamma);
	if (map->camera->projection == ISO)
		iso(&c.x, &c.y, c.z);
		c.x += (map->X_res - MENU_WIDTH) / 2 + map->camera->x_offset + MENU_WIDTH;
		c.y += (map->Y_res + map->height * map->camera->zoom) / 2 + map->camera->y_offset;
		return (c);
}

/*
** Rotate coordinate by x axis
*/

static void	rotate_x(int *y, int *z, double alpha)
{
	int previous_y;

	previous_y = *y;
	*y = previous_y * cos(alpha) + *z * sin(alpha);
	*z = -previous_y * sin(alpha) + *z * cos(alpha);
}


static void	rotate_y(int *x, int *z, double beta)
{
	int previous_x;

	previous_x = *x;
	*x = previous_x * cos(beta) + *z * sin(beta);
	*z = -previous_x * sin(beta) + *z * cos(beta);
}

/*
** Rotate coordinate by z axis
*/

static void	rotate_z(int *x, int *y, double gamma)
{
	int previous_x;
	int previous_y;

	previous_x = *x;
	previous_y = *y;
	*x = previous_x * cos(gamma) - previous_y * sin(gamma);
	*y = previous_x * sin(gamma) + previous_y * cos(gamma);
}

/*
** Convert coordinate to iso projection
*/

static void	iso(int *x, int *y, int z)
{
	int previous_x;
	int previous_y;

	previous_x = *x;
	previous_y = *y;
	*x = (previous_x - previous_y) * cos(0.523599);
	*y = -z + (previous_x + previous_y) * sin(0.523599);
}

/*
** Get color. Result depends on point position.
** This function is needed to create linear gradient.
*/

int	get_color(t_point current, t_point start, t_point end, t_point delta)
{
	int		red;
	int		green;
	int		blue;
	double	percentage;

	if (current.color == end.color)
		return (current.color);
	if (delta.x > delta.y)
		percentage = percent(start.x, end.x, current.x);
	else
		percentage = percent(start.y, end.y, current.y);
	red = get_light((start.color >> 16) & 0xFF,
					(end.color >> 16) & 0xFF,
					percentage);
	green = get_light((start.color >> 8) & 0xFF,
					(end.color >> 8) & 0xFF,
					percentage);
	blue = get_light(start.color & 0xFF,
					end.color & 0xFF,
					percentage);
	return ((red << 16) | (green << 8) | blue);
}

int	get_default_color(int z, t_map *map)
{
	double	percentage;

	percentage = percent(map->z_min, map->z_max, z);
	if (percentage < 0.2)
		return (COLOR_DISCO);
	else if (percentage < 0.4)
		return (COLOR_BRICK_RED);
	else if (percentage < 0.6)
		return (COLOR_FLAMINGO);
	else if (percentage < 0.8)
		return (COLOR_JAFFA);
	else
		return (COLOR_SAFFRON);
}

/*
** Get light for color. Result depends on point position.
** This function is needed to create linear gradient.
*/

int	get_light(int start, int end, double percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}


/*
** Get percent
*/

double	percent(int start, int end, int current)
{
	double	placement;
	double	distance;

	placement = current - start;
	distance = end - start;
	return ((distance == 0) ? 1.0 : (placement / distance));
}


/*
** Get index in array by x and y
*/

int		get_index(int x, int y, int width)
{
	return (y * width + x);
}

/*
** Create t_point element (x value + y value + z value + color value)
*/

t_point	new_point(int x, int y, t_map *map)
{
	t_point	point;
	int		index;

	index = get_index(x, y, map->width);
	point.x = x;
	point.y = y;
	point.z = map->coords[index];
	point.color = (map->colors[index] == -1) ?
			get_default_color(point.z, map) : map->colors[index];
	return (point);
}
