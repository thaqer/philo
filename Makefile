CC = cc

CFLAGS = -Wall -Wextra -Werror -g

NAME = philo

SRCS = src/main.c \
	src/utils.c \
	src/routine.c \
	src/monitor.c \
	src/initilayzation.c \

OBJ = $(addprefix objs/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) 

objs/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf objs

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re