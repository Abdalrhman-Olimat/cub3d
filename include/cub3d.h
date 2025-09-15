#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "../Libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include "defines.h"

// Color structure (RGB)
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

// Texture structure
typedef struct s_texture
{
	char			*path;
	mlx_texture_t	*texture;
}	t_texture;

// Player structure
typedef struct s_player
{
	double	x;				// Player x position
	double	y;				// Player y position
	double	dir_x;			// Direction vector x
	double	dir_y;			// Direction vector y
	double	plane_x;		// Camera plane x
	double	plane_y;		// Camera plane y
	char	orientation;	// Starting orientation (N, S, E, W)
}	t_player;

// Map structure
typedef struct s_map
{
	char	**grid;		// 2D map grid
	int	width;			// Map width
	int	height;			// Map height
}	t_map;

// Game state structure
typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_texture	textures[4];	// NO, SO, WE, EA textures
	t_color		floor_color;
	t_color		ceiling_color;
	t_map		map;
	t_player	player;
	char		**file_content;	// Raw file content
}	t_game;

// Function prototypes
// Parsing functions
int		parse_arguments(int argc, char **argv);
int		parse_file(char *filename, t_game *game);
int		parse_textures_and_colors(t_game *game);
int		parse_map(t_game *game);
int		validate_map(t_game *game);

// Texture and color parsing
int		parse_texture(char *line, t_texture *texture, char *identifier);
int		parse_color(char *line, t_color *color, char identifier);
int		validate_rgb(int r, int g, int b);

// Map validation
int		is_valid_map_char(char c);
int		is_player_char(char c);
int		find_player(t_game *game);
int		check_walls(t_game *game);

// Utility functions
char	**read_file(char *filename);
void	free_array(char **array);
void	cleanup_game(t_game *game);
void	error_exit(char *message);
char	*skip_whitespace(char *str);

// Game initialization
int		init_game(t_game *game);
void	init_player(t_game *game);

#endif