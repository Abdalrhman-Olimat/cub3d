#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../Libft/libft.h"
#include "../MLX42/include/MLX42/MLX42.h"

// Map characters
#define WALL '1'
#define EMPTY '0'
#define NORTH 'N'
#define SOUTH 'S'
#define EAST 'E'
#define WEST 'W'

// Window settings
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

// Math constants
#define PI 3.14159265359

// Error messages
#define ERR_ARGC "Error\nWrong number of arguments\n"
#define ERR_FILE_EXT "Error\nFile must have .cub extension\n"
#define ERR_FILE_OPEN "Error\nCannot open file\n"
#define ERR_FILE_READ "Error\nCannot read file\n"
#define ERR_MALLOC "Error\nMemory allocation failed\n"
#define ERR_TEXTURE "Error\nInvalid texture path\n"
#define ERR_COLOR "Error\nInvalid color value\n"
#define ERR_MAP "Error\nInvalid map\n"
#define ERR_PLAYER "Error\nInvalid player position\n"

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
	char		*path;
	mlx_texture_t	*texture;
}	t_texture;

// Player structure
typedef struct s_player
{
	double	x;		// Player x position
	double	y;		// Player y position
	double	dir_x;	// Direction vector x
	double	dir_y;	// Direction vector y
	double	plane_x;	// Camera plane x
	double	plane_y;	// Camera plane y
	char	orientation;	// Starting orientation (N, S, E, W)
}	t_player;

// Map structure
typedef struct s_map
{
	char	**grid;		// 2D map grid
	int	width;		// Map width
	int	height;		// Map height
}	t_map;

// Ray structure for raycasting
typedef struct s_ray
{
	double		camera_x;		// X-coordinate in camera space
	double		dir_x;			// Ray direction X
	double		dir_y;			// Ray direction Y
	int		map_x;			// Current map square X
	int		map_y;			// Current map square Y
	double		side_dist_x;		// Length to next X-side
	double		side_dist_y;		// Length to next Y-side
	double		delta_dist_x;		// Distance between X-sides
	double		delta_dist_y;		// Distance between Y-sides
	double		perp_wall_dist;		// Perpendicular wall distance
	int		step_x;			// Step direction X (-1 or 1)
	int		step_y;			// Step direction Y (-1 or 1)
	int		hit;			// Was a wall hit?
	int		side;			// Was it X-side or Y-side?
	int		line_height;		// Height of wall to draw
	int		draw_start;		// Start Y position for drawing
	int		draw_end;		// End Y position for drawing
	mlx_texture_t	*texture;		// Current texture
	int		tex_x;			// X coordinate on texture
	double		step_tex;		// Texture step per pixel
	double		tex_pos;		// Current texture position
}	t_ray;

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

// Graphics functions
int		init_graphics(t_game *game);
int		init_mlx(t_game *game);
int		init_image(t_game *game);
int		load_textures(t_game *game);
int		load_texture(t_game *game, t_texture *tex);
void	cleanup_graphics(t_game *game);

// Rendering functions
void	render_frame(t_game *game);
void	render_floor_ceiling(t_game *game);
void	put_pixel(mlx_image_t *img, int x, int y, uint32_t color);
void	clear_image(mlx_image_t *img, uint32_t color);
void	draw_vertical_line(mlx_image_t *img, int x, int y1, int y2, uint32_t color);
uint32_t	get_rgba(int r, int g, int b, int a);

// Game loop functions
void	game_loop(void *param);
void	start_game_loop(t_game *game);
void	close_hook(void *param);

// Raycasting functions
void	raycast(t_game *game);
void	cast_ray(t_game *game, int x);
void	init_ray(t_game *game, t_ray *ray, int x);
void	calculate_step(t_game *game, t_ray *ray);
void	perform_dda(t_game *game, t_ray *ray);
void	calculate_wall_distance(t_game *game, t_ray *ray);
void	calculate_wall_height(t_ray *ray);
void	calculate_texture_coords(t_game *game, t_ray *ray);
int	get_texture_index(t_ray *ray);

#endif