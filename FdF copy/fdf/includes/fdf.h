/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 14:59:17 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/05 17:09:29 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
#define FDF_H

#include "../libft/libft.h"
#include "minilibx/mlx.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define WINDOW_WIDTH   1200
#define WINDOW_HEIGH    800

# define ERR_USAGE			"usage: ./fdf [filename]"
# define ERR_MAP			"error: invalid map"
# define ERR_READING	"error: could not read the map"
# define ERR_MAP_INIT		"error: could not initialize map"
# define ERR_FDF_INIT		"error: could not initialize FdF"
# define ERR_CONV_TO_ARR	"error: conversion to array"
# define ERR_CAMERA_INIT	"error: camera initialization"

# define S1 "   FDF 42 project   "
# define S2 "To close the project: press ESC"
# define S3 "Move mouse wheel: zoom in/zoom out"
# define S4 "Press C: to change the color"
# define S5 "Press Z: to increase the depth of the point"

# define TEXT_COLOR			0xEAEAEA
# define BACKGROUND			0x222222
# define MENU_BACKGROUND	0x1E1E1E

# define COLOR_DISCO		0x9A1F6A
# define COLOR_BRICK_RED	0xC2294E
# define COLOR_FLAMINGO		0xEC4B27
# define COLOR_JAFFA		0xEF8633
# define COLOR_SAFFRON		0xF3AF3D


# define 3D_MAP (*winmap)->map->point

enum		e_mouse
{
	none,
	left,
	right,
	mid,
	scr_up,
	scr_down
};

typedef struct		xyz_map
{
	int 	x;
	int 	y;
	int		**z;
	char 	**color;
	int		newx;
	int		newy;
	struct	t_xyz	*next;
	struct 	t_xyz 	*right;
	struct  t_xyz 	*down;
}							t_xyz;


typedef struct		s_map
{
	void	*win;
	void	*mlx;
	t_map	*map;
	int		row;
	int		col;
	int		mouse;
	int		rotate;
} 						t_map;



void	kill_bill(char *s)

#endif