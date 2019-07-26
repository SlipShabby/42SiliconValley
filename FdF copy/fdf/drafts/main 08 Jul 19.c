/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 19:41:40 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/08 15:19:35 by ajulanov         ###   ########.fr       */
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
# define ERR_READ	    	"error: could not read the map"
# define ERR_CREATE_MAP		"error: could not initialize map"
# define ERR_FDF			"error: FDF initialization"
# define ERR_CAM			"error: camera initialization"



// Initial parameters
# define PRJ_NAME		"42 FDF"
# define INIT_X			1920
# define INIT_Y			1080
# define MENU_WIDTH		250

# define BCKGRND_CLR	0x2a2a2a
# define TEXT_CLR		0xe0e0e0
# define BASE_CLR		0xe8e8e8
# define TOP_CLR		0xe0e0e0
# define SCALE			50
# define SCALE_MULTI	1
# define ANGLE_X		-1.0
# define ANGLE_Y		0.0
# define ANGLE_Z		0.75
# define FOCAL			2000
# define BACK_A			0
# define PIXEL_A		0
# define BACKGROUND			0x222222
# define MENU_BACKGROUND	0x1E1E1E

# define FT_MIN(A, B) (((A) < (B)) ? (A) : (B))
# define FT_MAX(A, B) (((A) > (B)) ? (A) : (B))

# define FT_ABS(X) (((X) < 0) ? (-(X)) : (X))

# define ULONG_MAX	((unsigned long)(~0L))
# define LONG_MAX	((long)(ULONG_MAX >> 1))
# define LONG_MIN	((long)(~LONG_MAX))

# define UINT_MAX	((unsigned)(~0L))
# define INT_MAX		((int)(UINT_MAX >> 1))
# define INT_MIN		((int)(~INT_MAX))

#define CENTER		

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
}                     		  		t_point;

typedef struct s_image
{
	void				*img_ptr;
	char				*img_addr;
	int					b_p_p;
	int					l_s;
	int					e;
	int					background;
	int					text_color;
	int					base_color;
	int					top_color;
	unsigned char		back_alpha;
	unsigned char		pix_alpha;
}									t_image;

typedef struct		s_xy
{
	int					x;
	int					y;
}									t_xy;

typedef struct s_mouse
{
	char				is_pressed;
	int					x1;
	int					y1;
	int					x2;
	int					y2;
}									t_mouse;

typedef struct s_set
{
	char				mouse_3_press;
	t_xy				mouse_xy;
	char				mouse_1_press;
	char				perspective;
	int					focal_lenght;
	char				distortion_1;
	char				distortion_2;
	char				distortion_3;
	char				depth;
	char				info;
	char				help;
	int					scale_mult;
	int					m_blur;
} 									t_set;

typedef struct s_camera
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

	// store z range for color gradient
	int	    			z_min;
	int					z_max;
	int					z_range;

	// draw
	int					bits_per_pixel;
	int 				scale;
	char				*data;
	double				angle_x;
	double				angle_y;
	double				angle_z;
	t_image				img;

	// env settings

	t_set				params;
	t_mouse				*mouse;
	t_camera			*camera;
} 									t_map;


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
void 				draw_map(t_map *map);
int					deal_key(int key, void *param);
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
		draw_map(map); 
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
	map->z_max = INT_MAX;
	map->z_min = INT_MIN;
	map->z_range = 0;
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
	// 	for(int ii = 0; ii < map->height; ii++)
	// {
	// 	for (int nn = 0; nn < map->width; nn++)
	// 		printf("%d,%d ", map->map_z[ii][nn].z, map->map_z[ii][nn].color);
	// 	printf("\n");
	// }
	printf("checkpoint: get_z + color\n");
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

void get_z3(t_map *map)
{
	int i;

	i = 0;
	map->z_min = 0;
	map->z_max = 0;
	map->z_range = 0;
}



//________________________________________________________________________
//
// DRAW a MAP
// _______________________________________________________________________


void draw_map(t_map *map);
{
	
	prms = ft_memalloc(sizeof(t_glob));
	map->mlx = mlx_init();
    map->win = mlx_new_window(map->mlx, INIT_X, INIT_Y, PRJ_NAME);
	if (!(fdf = (t_map *)ft_memalloc(sizeof(t_map))))
	
}




 

// void init_window(t_map *map)
// {
// 	map->params.info = 1;
// 	map->img.background = BCKGRND_CLR;
// 	map->img.text_color = TEXT_CLR;
// 	map->img.base_color = BASE_CLR;
// 	map->img.top_color = TOP_CLR;
// 	map->img.back_alpha = BACK_A;
// 	map->img.pix_alpha = PIXEL_A;
// 	map->scale = SCALE;
// 	map->params.scale_mult = SCALE_MULTI;
// 	map->params.focal_lenght = FOCAL;
// 	map->angle_x = ANGLE_X;
// 	map->angle_y = ANGLE_Y;
// 	map->angle_z = ANGLE_Z;
// }


// void draw_map(t_map *map)
// {
// 	int x;
//  int y;
// 	t_point	v;
// 	char *line;

    // map->mlx = mlx_init();
    // map->win = mlx_new_window(map->mlx, INIT_X, INIT_Y, PRJ_NAME);
//     x = 0;
// 	while (x <= map->width)
//     {
// 		y = 0;
// 		while (y < map->height)
// 		{
// 			mlx_pixel_put(map->mlx, map->win, x, y, 0x0FF0FF);
// 			y++;
// 		}
//         x++;
//     }
// 	mlx_key_hook(map->win, deal_key, (void *)0);
//     mlx_loop(map->mlx);
// }

// int deal_key(int key, void *param)
// {
//     ft_putchar('X');
// 	return (0);
// }



// void draw_map(t_map *map)
// {
// 	int x;
//     int y;

//     x = 0;
//     y = 250;
//     map->mlx = mlx_init();
//     map->win = mlx_new_window(map->mlx, INIT_X, INIT_Y, PRJ_NAME);
//     while (x <= INIT_X)
//     {
//        mlx_pixel_put(map->mlx, map->win, x, y, 0x0FF0FF);
//        x++;
//     }
//     mlx_key_hook(map->win, deal_key, (void *)0);
//     mlx_loop(map->mlx);
// }

// int deal_key(int key, void *param)
// {
//     ft_putchar('X');
// 	return (0);
// }


// void draw_map(t_map *map)
// {
// 	int x;
// 	int y; 

// 	draw_background(map);
// 	y = 0;
// 	while (y < map->height)
// 	{
// 		x=0;
// 		while (x < map->width)
// 		{
// 			if (x != map->width - 1)
// 				draw_line(project(new_point(x,y,map) map), \
// 					project(new_point(x+1, y, map), map), map);
// 			if (y != map->height - 1)
// 				draw_line(new_point(x,y,map) map), \
// 					project(new_point(x, y+1, map), map), map);
// 			x++;
// 		}
// 		y++;
// 	}
// 	mlx_put_image_to_window(map->mlx, map->win, map->img, 0, 0);
// 	print_menu(map);
// }

// void draw_background(t_map *map)
// {
// 	int *image;
// 	int i;

// 	ft_bzero(map->data, INIT_X * INIT_Y * (map->bits_per_pixel / 8));
// 	image = (int *)(map->data);
// 	i=0;
// 	while (i < INIT_X * INIT_Y)
// 	{
// 		image[i] = (i % INIT_X < MENU_WIDTH) ? MENU_BACKGROUND : BACKGROUND;
// 		i++;
// 	}
// }

// void draw_line(t_point f, t_point s, t_map *map)
// {
// 	t_point delta;
// 	t_point sign;
// 	t_point current;
// 	int error[2];

// 	delta.x = FT_ABS(s.x - f.x);
// 	delta.y = FT_ABS(s.y - f.y);
// 	sign.x = f.x < s.x ? 1 : -1;
// 	sign.y = f.x < s.y ? 1 : -1;
// 	error[0] = delta.x - delta.y;
// 	current = f;
// 	while (current.x != s.x || current != s.y)
// 	{
// 		put_pixel(map, current.x, current.y, get_z(map))
// 	}
// }