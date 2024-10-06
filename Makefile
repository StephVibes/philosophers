CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c src/utils.c src/init.c src/monitor.c src/time.c src/routine.c
OBJS = $(SRCS:.c=.o)
NAME = philo

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all