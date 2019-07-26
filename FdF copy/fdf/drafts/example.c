/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 15:17:59 by ajulanov          #+#    #+#             */
/*   Updated: 2019/07/14 14:20:10 by ajulanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minilibx/mlx.h"
#include <unistd.h>

void ft_putchar(char c)
{
    write(1, &c,1);
}

int deal_key(int key, void *param)
{
    ft_putchar('X');
    return (0);
}

int main()
{
    void *mlx_ptr;
    void *win_ptr;
    int x;
    int y;

    x = 0;
    y = 250;
    mlx_ptr = mlx_init();
    win_ptr = mlx_new_window(mlx_ptr, 500, 500, "mlx 42");
    while (x <= 500)
    {
       mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x0FF0FF);
       x++;
    }
    
   // mlx_string_put(mlx_ptr, deal_key, 250, 250, 0x0FF0FF, "mlx 42");

    mlx_key_hook(win_ptr, deal_key, (void *)0);
    mlx_loop(mlx_ptr);
    return(0);
}