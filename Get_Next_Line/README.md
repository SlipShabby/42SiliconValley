# Get_Next_Line

Whether it's a file, standard input, or even later a network connection, you'll still need to read content line by line. It is therefore time to tackle this function, which is essential for a number of your next projects.

## How to use?

cd get_next_line
make -C libft/

## Build the executable:

gcc -Wall -Wextra -Werror -I./libft/ -L./libft -lft -o gnl get_next_line.c main.c

## Now test it with:

./gnl test.txt
