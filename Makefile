NAME = libcmap
NAME_AR = $(NAME).a
NAME_DYN = $(NAME).so

_SRC = cmap.c

_OBJ = $(_SRC:.c=.o)

CC = gcc
CFLAGS += -Wall -Wextra -Werror
.PHONY: all clean fclean res test
all: $(NAME)

$(NAME): $(NAME_AR) $(NAME_DYN)

$(NAME_DYN): $(_OBJ)
	$(CC) -shared $^ -o $@

$(NAME_AR): $(_OBJ)
	ar -rcs $@ $^
	ranlib $@
%.o: %.c
	$(CC)  $(CFLAGS) -I./ -c $< -o $@

clean : 
	rm -f $(_OBJ)
fclean: clean
	rm -f $(NAME).*

re: fclean all

test:
	echo "cmap test"
