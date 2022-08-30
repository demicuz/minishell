NAME		:= minishell

SRC_DIR		:= src
LIB_DIR		:= lib

LIBFT_DIR	:= libft

LIBFT		:= $(LIB_DIR)/libft.a

SRC			:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ			:= $(SRC:.c=.o)

# TODO Set correct flags
CC			:= cc
CPPFLAGS	:= -I include -I $(LIBFT_DIR) -MMD -MP -O0 -g
CFLAGS		:= -Wall#-Werror -Wextra
LDFLAGS		:= -L $(LIB_DIR)
LDLIBS		:= -lft -lreadline#-l:libft

.PHONY: all bonus clean fclean re

all: $(NAME)

bonus: $(NAME)

$(LIBFT): $(LIB_DIR)
	$(MAKE) --directory=$(LIBFT_DIR)
	cp $(LIBFT_DIR)/libft.a $(LIB_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)

# Turns out you don't have to have a rule for object files. 'Make' uses CFLAGS
# and CPPFLAGS variables to compile .o files.
# %.o: %.c
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_DIR):
	mkdir -p $@

clean:
	rm -vf $(OBJ)
	rm -vf $(OBJ:.o=.d)
	$(MAKE) clean --directory=$(LIBFT_DIR)

fclean: clean
	rm -vf $(NAME)
	$(MAKE) fclean --directory=$(LIBFT_DIR)

re: fclean all

-include $(OBJ:.o=.d)
