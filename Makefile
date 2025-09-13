NAME = cub3d
CC = cc
CFLAG = -Wall -Werror -Wextra
SRC_PATH = src/
OBJ_PATH = obj/
SRC = main.c parsing.c texture_color_parsing.c map_parsing.c
LIBFT = Libft/libft.a
INCS	= -I ./include/ -IMLX42/include
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

all: $(OBJ_PATH) $(NAME) 

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -c $< -o $@ $(INCS)

$(OBJ_PATH):
	mkdir $(OBJ_PATH)


MLX42_LIB = MLX42/build/libmlx42.a
MLX42_INC = -IMLX42/include
MLX42_FLAGS = -ldl -lglfw -pthread -lm

$(NAME): $(OBJS) $(LIBFT) $(MLX42_LIB)
	make all -C Libft
	$(CC) $(CFLAG) $(OBJS) -I./Libft $(LIBFT) $(MLX42_LIB) $(INCS) $(MLX42_INC) $(MLX42_FLAGS) -o $(NAME)

$(LIBFT):
	make -C Libft  

clean:
	rm -rf $(OBJ_PATH) $(NAME)
	make clean -C Libft
fclean: clean
	rm -rf $(NAME)
	make fclean -C Libft

re: fclean all