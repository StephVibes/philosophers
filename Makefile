CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c utils.c
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