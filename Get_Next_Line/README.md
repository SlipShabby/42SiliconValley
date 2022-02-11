# Get_Next_Line (GNL)

Whether it's a file, standard input, or even later a network connection, you'll still need to read content line by line. It is therefore time to tackle this function, which is essential for a number of your next projects (fdf, fractol, etc).

Projects shows in basic low level how to work with memory allocation. The program opens a file, reads lines from it with a custom function get_next_line() that uses unix system calls and malloc to return arbitrary sized lines. It then prints these lines using the unix system call and appends a newline with a separate system call. Also GNL will automatically free appropriately whenever it reaches EOF. My GNL works with multiple file descriptors without memory leaks.

Disclaimer: Implementation of analog in python:

with open('filename') as file: 
    lines = file.readline()  


## How to use?

cd get_next_line
make -C libft/

## Build the executable:

gcc -Wall -Wextra -Werror -I./libft/ -L./libft -lft -o gnl get_next_line.c main.c

## Now test it with:

./gnl test.txt

