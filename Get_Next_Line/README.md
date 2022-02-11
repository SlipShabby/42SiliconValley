# Get_Next_Line (GNL)

Whether it's a file, standard input, or even later a network connection, you'll still need to read content line by line. It is therefore time to tackle this function, which is essential for a number of your next projects (fdf, fractol, etc).
Projects shows in basic low level how to work with memory allocation. Also GNL will automatically free appropriately whenever it reaches EOF.

My GNL works with multiple file descriptors without memory leaks.

Disclaimer: It is implementation of file.readline() [python]. 


## How to use?

cd get_next_line
make -C libft/

## Build the executable:

gcc -Wall -Wextra -Werror -I./libft/ -L./libft -lft -o gnl get_next_line.c main.c

## Now test it with:

./gnl test.txt

