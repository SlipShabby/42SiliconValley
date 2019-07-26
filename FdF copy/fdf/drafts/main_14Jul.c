/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 19:41:40 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/13 23:38:09 by ajulanov         ###   ########.fr       */
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
}									t_xy;

typedef struct		s_hooks
{
	int				zr;
	int				yr;
	int				xr;
	int				xt;
	int				yt;
	int				zs;
}					t_hooks;

typedef	struct		s_bresen
{
	float				offset;
	float				threshold;
	float				adjust;
	float				delta;
	float				tmp;
	int					colorgrade;
	float				color;
	float				range;
	char				*str;
	char				**coord;
	char				*line;
}					t_bresen;

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

	t_hooks				*hooks;
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
void				hooks(t_hooks **hooks);
t_point				**make_carte(t_map *map);
void 				slope1(t_map *map);
void				slope2(t_map *map, t_bresen *brs);
void				slope3(t_map *map, t_bresen *brs);
void				draw_map(t_map *map, t_hooks *hooks);
void				draw_right(t_map *map, t_bresen **brs, t_hooks *hooks);
void				draw_down(t_map *map, t_bresen **brs, t_hooks *hooks);
void				set_right(t_map *map, int i, int j, t_hooks *hooks);
void				set_down(t_map *map, int i, int j, t_hooks *hooks);
int					ft_color(t_map *map, t_bresen *brs, float distance);
int					ft_color1(t_map *map, t_bresen *brs, float distance);
int					ft_color2(t_map *map, t_bresen *brs, float distance);
int					ft_color3(t_map *map, t_bresen *brs, float distance);
void				rotate(t_map *rotated, t_hooks *hooks);
void				rotate_x(t_map *rotated);
void				rotate_y(t_map *rotated);
void				rotate_z(t_map *rotated);
void				centered(t_map *rotated, t_hooks *hooks);

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
			map->map_z[h][i].x = x;
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
	map->hooks = (t_hooks *)malloc(sizeof(t_hooks*));
	hooks(&map->hooks);
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
		put_pixel(map, recent.x, recent.y, get_color(recent, a, b, delta));
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