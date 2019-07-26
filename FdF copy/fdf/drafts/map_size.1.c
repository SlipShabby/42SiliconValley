/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 16:54:44 by ajulanov          #+#    #+#             */
/*   Updated: 2019/06/25 18:20:06 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft/libft.h"
#include "../GNL/get_next_line.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

# define ERR_USAGE			"usage: ./fdf [filename]"
# define ERR_MAP		    "error: invalid map"
# define ERR_READING	    "error: could not read the map"
# define ERR_MAP_INIT		"error: could not initialize map"

typedef struct		s_map
{
	// void	*win;
	// void	*mlx;
	int		height;
	int		width;
    int     *colors;
    int     *coords;
	// int		mouse;
	// int		rotate;
    int     fd;
} 						t_map;

void	kill_bill(char *s);



void	kill_bill(char *s)
{
	if (errno == 0)
		ft_putendl_fd(s, 2);
	else
		perror(s);
	exit(1);
}

t_map *create_map(void)
{
    t_map *map;

    if(!(map = (t_map *)ft_memalloc(sizeof(t_map)))
        kill_bill(ERR_MAP_INIT);
    map->width = 0;
    map->height = 0;
    map->colors = NULL;
    map->coords = NULL;
    return (map);
}

void get_map(t_map *map, char *file_name)
{
    if ((map->fd = open(file_name, O_RDONLY)) < 0)
        kill_bill(ERR_READING);
    get_xy(map);
    close(map->fd);
    if ((map->fd = open(file_name, O_RDONLY) < 0))
        kill_bill(ERR_READING);
    get_z(map);
    close(map->fd);
}

void check_first_line(t_map *map, char *line)
{

}

int main(int ac, char **av)
{
    t_map   *map;
    int fd;
    
    if (ac == 2)
    {
        if (!((map->fd = open(av[1], O_RDONLY)) >= 0))
			kill_bill(ERR_READING);
		map = create_map();
        get_map(map, av[1]);
        read_map(map);
    }
    kill_bill(ERR_USAGE);
	return (0);
}


// int main()
// {
// 	char *line;
//     t_map   *map;
//     int fd;
//     int i;
//     int count_width;
//     int first_line;

//     first_line = 1;
         
//     map = ft_memalloc(sizeof(t_map));
//     fd = open("../maps/42_2.fdf", O_RDONLY);

// 	while ((get_next_line(fd, &line)) == 1)
// 	{
//         i = 0;
//         count_width = 0;   
//         while (line[i])
//         {
//             if (line[i] == ' ' )
//             i++;
//             else if (line[i] =='\n')
//             {
//                 break ;
//             }
//             else 
//                 {
//                     while (line[i] >= '0' && line[i] <= '9')
//                         i++;
//                     count_width++;
//                 }
//         }
//         if (first_line)
//         {
//             map->col = count_width;
//             first_line = 0;
//         }
//         else if (count_width != map->col)
//         {
//             kill_bill(ERR_MAP);
//             return(-1);
//         }
//         printf("%d \n", map->col);
//         map->row++;
// 	}
//     printf("%d \n", map->row);
//     return(map->row);
// }
