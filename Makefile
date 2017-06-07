
NAME = wolf3d

HEADER = wolf.h

SRC = main.c colour.c print.c additional.c map.c key_funk.c rawcast_calc.c

OBJ = ${SRC:.c=.o}

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

%.o : %.c $(HEADER)
	@gcc  -c -o $@ $< -I -lmlx

$(NAME): $(OBJ)
	@gcc -o $@ $(OBJ) -lmlx -framework OpenGL -framework AppKit -lm
	@echo "\033[92mCompilation is done\033[0m"
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
