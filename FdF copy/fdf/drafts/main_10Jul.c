/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_10Jul.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 19:41:40 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/11 11:23:18 by ajulanov         ###   ########.fr       */
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

# define FT_MIN			(A, B) (((A) < (B)) ? (A) : (B))
# define FT_MAX			(A, B) (((A) > (B)) ? (A) : (B))

# define FT_ABS			(X) (((X) < 0) ? (-(X)) : (X))

# define ULONG_MAX		((unsigned long)(~0L))
# define LONG_MAX		((long)(ULONG_MAX >> 1))
# define LONG_MIN		((long)(~LONG_MAX))

# define UINT_MAX		((unsigned)(~0L))
# define INT_MAX		((int)(UINT_MAX >> 1))
# define INT_MINI		((int)(~INT_MAX))

# define CENTER_X		map->width/2;
# define CENTER_Y		map->height/2;

# define MOVE_X			((map->k - map->mid_x) * map->gap);
# define MOVE_Y			((map->m - map->mid_y) * map->gap);
	

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
	// int					rawz;
	int 				color;
}                     		  		t_point;

typedef struct		s_hooks
{
	int				zr;
	int				yr;
	int				xr;
	int				xt;
	int				yt;
	int				zs;
}					t_hooks;


typedef struct		s_xy
{
	int					x;
	int					y;
}									t_xy;


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
	float				Y_res;	// init win Y

	
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
// void 				get_color(t_map *map);
t_bool				ft_isnumber(char *str, int base);
// void 				set_env(t_map *map);

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
		// set_env(map); 
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
	// map->z_range = 0;
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


// void set_env(t_map *map)
// {
// 	map->mlx = mlx_init();
//     map->win = mlx_new_window(map->mlx, INIT_X, INIT_Y, PRJ_NAME);
// 	if (!(fdf = (t_map *)ft_memalloc(sizeof(t_map))))
// 		kill_bill(ERR_FDF);
// 	// set_default(map);
// 	draw(map, hooks);
// }


// void draw(t_map *map, t_hooks *hooks)
// {
// 	int x;
// 	int y;
// 	t_bresen *draw;

// 	x= 0;
// 	y = 0;
// 	draw = (t_bresen *)malloc(sizeof(t_bresen));
// 	rotate(map, hooks);
// 	translate(map, hooks);
// 	draw_right(map, hooks);
// 	draw_down(map, hooks);
// }

// void	hooks(t_hooks **hooks)
// {
// 	/* Hooks dont do anything but its there to give you an idea of where to use it and how for bonus */
// 	(*hooks)->zr = 1;
// 	(*hooks)->yr = 1;
// 	(*hooks)->xr = 1;
// 	(*hooks)->xt = 0.0;
// 	(*hooks)->yt = 0.0;
// 	(*hooks)->zs = 0;
// 	printf("hooks finished\n");
// }




// void set_default(t_map *map)
// {
// 	map->x1 = 0; 
// 	map->y1 = 0;
// 	map->x2 = 0; 
// 	map->y2 = 0;
// 	map->big = map->width > map->height ? map->width : map->height;
// 	map->gap = map->width > map->height ? 700 / (map->width) : 700 (map->height);
// 	map->s = 0;
// 	map->rise = 0;
// 	map->run = 0;
// 	map->max = 800 + map->gap * map->big;
// 	map->maxside = map->max - 300;
// 	map->k = 0;
// 	map->m = 0;
// 	map->cart = make_cartesian(*map, s);
// 	hooks (map->hooks)
// }




//  t_points		**ft_makecartesian(t_map *map)
// {
// 	t_points	**c;
	
// 	if (!(c = (t_points **)malloc(sizeof(t_points *) * (m->h + 1))))
// 		return (NULL);
// 	m->midx = m->w / 2;
// 	m->midy = m->h / 2;
// 	while (fdf->i < m->h)
// 	{
// 		c[fdf->i] = (t_points *)malloc(sizeof(t_points) * (m->w + 1));
// 		fdf->j = 0;
// 		while (fdf->j < m->w)
// 		{
// 			c[fdf->i][fdf->j].x = ((fdf->j - m->midx) * fdf->gap);
// 			c[fdf->i][fdf->j].y = ((fdf->i - m->midy) * fdf->gap);
// 			c[fdf->i][fdf->j].z = m->map[fdf->i][fdf->j] * 5;
// 			c[fdf->i][fdf->j].rawz = m->map[fdf->i][fdf->j];
// 		 	fdf->j++;
// 		}
// 		fdf->i++;
// 	}
// 	c[fdf->i] = NULL;
// 	printf("x/width = %d, y/height = %d\n", m->w, m->h);
// 	printf("midx = %d, midy = %d\n", m->midx, m->midy);
// 	printf("(0,0) = (%f, %f)\n", c[0][0].y, c[0][0].x);
// 	printf("(0,1) = (%f, %f)\n", c[0][1].y, c[0][1].x);
// 	printf("(1,0) = (%f, %f)\n", c[1][0].y, c[1][0].x);
// 	printf("(%d,%d) = (%f, %f)\n", m->midy, m->midx, c[m->midy][m->midx].y, c[m->midy][m->midx].x);
// 	printf("(9,14) = (%f, %f)\n", c[9][14].y, c[9][14].x);
// 	return (c);
// }



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