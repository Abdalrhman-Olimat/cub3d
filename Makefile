NAME = cub3D
CC = cc
CFLAG = -Wall -Werror -Wextra
SRC_PATH = src/
HELPERS_PATH = src/Helpers/
OBJ_PATH = obj/
SRC = main.c parsing.c texture_color_parsing.c map_parsing.c \
      graphics_init.c rendering.c game_loop.c raycasting.c input.c
HELPERS = input_helper.c map_parsing_core.c map_validation_helper.c \
	  map_grid_helper.c map_grid_alloc.c player_finder.c player_init.c \
	  wall_check_helper.c flood_fill_logic_helper.c file_reading_helpers.c \
	  utility_helpers.c texture_parsing_helpers.c color_parsing_helpers.c \
	  texture_color_main_parsing.c rendering_helpers.c rendering_floor_ceiling.c \
	  raycasting_helpers.c raycasting_texture_helpers.c raycasting2.c
LIBFT = Libft/libft.a
INCS	= -I ./include/ -IMLX42/include
SRCS	= $(addprefix $(SRC_PATH), $(SRC)) $(addprefix $(HELPERS_PATH), $(HELPERS))
OBJ		= $(SRC:.c=.o)
HELPERS_OBJ = $(HELPERS:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ)) $(addprefix $(OBJ_PATH), $(HELPERS_OBJ))

all: $(OBJ_PATH) $(NAME) 

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -c $< -o $@ $(INCS)

$(addprefix $(OBJ_PATH), $(HELPERS_OBJ)): $(OBJ_PATH)%.o: $(HELPERS_PATH)%.c
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

.PHONY: all clean fclean re