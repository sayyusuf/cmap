NAME = cmap.a

_SRC = cmap.c

_OBJ = $(_SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror 

.PHONY: all clean fclean res test
all: $(NAME)

$(NAME): $(_OBJ)
	ar -rcs $@ $^
	ranlib $@
%.o: %.c
	$(CC)  $(FLAGS) -I./ -c $< -o $@

clean : 
	rm -f $(_OBJ)
fclean: clean
	rm -f $(NAME)

re: fclean all

test:
	echo "cmap test"
