# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajulanov <ajulanov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/18 19:03:30 by ajulanov          #+#    #+#              #
#    Updated: 2019/06/18 19:05:07 by ajulanov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= gnl

SRC= main.c get_next_line.c
OBJ= $(SRC:.c=.o)

CC= gcc
CFLAGS= -Wall -Wextra -Werror

LIBFT= ../libft/libft.a
LIBINC= -I../libft
LIBLINK= -L../libft -lft
INC= get_next_line.h

all: $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) $(LIBINC) -I $(INC) -o $@ -c $<

libft: $(LIBFT)

$(LIBFT):
	@make -C ./libft

$(NAME): libft $(OBJ) 
	@$(CC) $(LIBLINK) -o $(NAME) $(OBJ)

debug: libft $(OBJ) 
	@$(CC) $(LIBLINK) -g $(SRC)

remlib:
	@rm -rf $(LIBFT)

remoblib:
	@make fclean -C ./libft/

clean: remoblib
	@rm -rf $(OBJ)

fclean: clean remlib
	@rm -rf $(NAME)

re: fclean all

test: re
	@./gnl test.txt 

run-debug: debug
	@lldb a.out

run: re
	./gnl test.txt 
	./gnl test.txt test2.txt