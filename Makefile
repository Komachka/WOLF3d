
NAME = wolf3d

HEADER = wolf.h

SRC = main.c colour.c print.c additional.c

OBJ = ${SRC:.c=.o}

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

%.o : %.c $(HEADER)
	gcc  -c -o $@ $< -I ../minilibx/ -lm

$(NAME): $(OBJ)
	gcc -o $@ $(OBJ) -lmlx -lXext -lX11 -L ../minilibx/ -I ../minilibx/ -lm
	@echo "COMPILETION GOOD"
clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
