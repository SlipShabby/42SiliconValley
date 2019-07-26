# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/05 16:21:57 by ajulanov          #+#    #+#              #
#    Updated: 2019/07/05 16:57:34 by ajulanov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf

HEADER = fdf.h

FILES = main.c 

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

OBJ_DIR = ./obj/
SRC_DIR = ./src/
MLX_LIB = ./minilibx/libmlx.a

SRC=$(addprefix $(SRC_DIR), $(FILES))

all: path $(NAME) $(MLX_LIB)

path:
	mkdir -p $(OBJ_DIR)
	
$(NAME): 
	# gcc -c $(NAME) $(SRC) -L minilibx/ -lmlx -framework OpenGL -framework AppKit
	gcc -o $(NAME) $(SRC) -L minilibx/ -lmlx -framework OpenGL -framework AppKit
	# gcc -c $(NAME) $(SRC) -L minilibx/ -lmlx -framework OpenGL -framework AppKit

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(C) $(SRC) -o $@ -c $<

clean:
	# @echo "$(YELLOW) cleaning... $(CYAN)"
	# rm -rf $(OBJ_DIR)
	# make -C $(FT) clean
	# make -C $(MLX) clean

fclean: #clean
	# @echo "$(YELLOW) fcleaning... $(CYAN)"
	# rm -rf $(OBJ_DIR)
	rm -f $(NAME)
	# make -C $(FT) fclean

re: fclean all

// gcc -g main.c ../libft/libft.a ../GNL/get_next_line.c -L ../minilibx/ -lmlx -framework OpenGL -framework AppKit