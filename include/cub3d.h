/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeleimat <aeleimat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 19:36:40 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/08 17:25:16 by aeleimat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include "../Libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"

# define WALL '1'
# define EMPTY '0'
# define NORTH 'N'
# define SOUTH 'S'
# define EAST 'E'
# define WEST 'W'

# define WIN_WIDTH 1024
# define WIN_HEIGHT 768

# define PI 3.14159265359

# define ERR_ARGC "Error\nWrong number of arguments\n"
# define ERR_FILE_EXT "Error\nFile must have .cub extension\n"
# define ERR_FILE_OPEN "Error\nCannot open file\n"
# define ERR_FILE_READ "Error\nCannot read file\n"
# define ERR_MALLOC "Error\nMemory allocation failed\n"
# define ERR_TEXTURE "Error\nInvalid texture path\n"
# define ERR_COLOR "Error\nInvalid color value\n"
# define ERR_MAP "Error\nInvalid map\n"
# define ERR_PLAYER "Error\nInvalid player position\n"

typedef struct s_color
{
	int			r;
	int			g;
	int			b;
}	t_color;

typedef struct s_texture
{
	char			*path;
	mlx_texture_t	*texture;
}	t_texture;

typedef struct s_player
{
	double			x;
	double			y;
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
	char			orientation;
}	t_player;

typedef struct s_map
{
	char			**grid;
	int				width;
	int				height;
}	t_map;

typedef struct s_ray
{
	double			camera_x;
	double			dir_x;
	double			dir_y;
	int				map_x;
	int				map_y;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	double			perp_wall_dist;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
	mlx_texture_t	*texture;
	int				tex_x;
	double			step_tex;
	double			tex_pos;
}	t_ray;

typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_texture		textures[4];
	t_color			floor_color;
	t_color			ceiling_color;
	t_map			map;
	t_player		player;
	char			**file_content;
}	t_game;
typedef uint32_t	t_ui32;

int		parse_arguments(int argc, char **argv);
int		parse_file(char *filename, t_game *game);
int		parse_textures_and_colors(t_game *game);
int		parse_map(t_game *game);
int		validate_map(t_game *game);

int		parse_texture(char *line, t_texture *texture, char *identifier);
int		parse_color(char *line, t_color *color, char identifier);
int		validate_rgb(int r, int g, int b);

int		is_valid_map_char(char c);
int		is_player_char(char c);
int		is_texture_or_color_line(char *line);
int		find_player(t_game *game);
int		check_walls(t_game *game);
int		is_wall_or_void(t_game *game, int x, int y);
int		has_adjacent_space(t_game *game, int x, int y);
void	free_visited(char **visited, int height);
int		validate_visited_cell(t_game *game, char **visited, int i, int j);
void	mark_reachable_floors(t_game *game, char **visited, int x, int y);
int		flood_fill_check(t_game *game, char **visited, int x, int y);

int		find_map_start(t_game *game);
void	calculate_map_dimensions(t_game *game, int map_start);
int		allocate_and_fill_grid(t_game *game, int map_start);

void	init_player(t_game *game);

char	**allocate_visited_array(t_game *game);
int		validate_all_visited_cells(t_game *game, char **visited);
void	cleanup_visited_array(char **visited, int height);

int		is_boundary_position(t_game *game, int x, int y);
int		flood_fill_recursive_calls(t_game *game, char **visited, int x, int y);
int		handle_space_cell(t_game *game, char **visited, int x, int y);
int		handle_empty_cell(t_game *game, char **visited, int x, int y);

char	**read_file(char *filename);
void	free_array(char **array);
void	cleanup_game(t_game *game);
void	error_exit(char *message);
char	*skip_whitespace(char *str);

char	**add_line_to_content(char **content, char *line, int line_count);
int		open_file_fd(char *filename);
int		process_file_lines(int fd, char ***content);
int		validate_file_content(int line_count);

void	trim_trailing_whitespace(char *str);
int		validate_texture_file_access(char *path_start);
int		validate_texture_extension(char *path_start);

int		parse_rgb_values(char **rgb_split, t_color *color);
int		validate_textures_and_colors(int texture_count, int floor_set,
			int ceiling_set);
int		is_valid_color_char(char c);
int		validate_color_string(char *rgb_start);
char	*get_rgb_start(char *line);
int		validate_rgb_array(char **rgb_split);
int		set_color_values(t_color *color, int r, int g, int b);
int		handle_texture_line(char *line, t_game *game, int *texture_count);
int		handle_texture_line2(char *line, t_game *game, int *texture_count);
int		handle_color_line(char *line, t_game *game, int *floor_set,
			int *ceiling_set);

int		init_game(t_game *game);

int		init_graphics(t_game *game);
int		init_mlx(t_game *game);
int		init_image(t_game *game);
int		load_textures(t_game *game);
int		load_texture(t_game *game, t_texture *tex);
void	cleanup_graphics(t_game *game);

void	render_frame(t_game *game);
void	render_floor_ceiling(t_game *game);
void	put_pixel(mlx_image_t *img, int x, int y, uint32_t color);
void	clear_image(mlx_image_t *img, uint32_t color);

void	game_loop(void *param);
void	start_game_loop(t_game *game);
void	close_hook(void *param);

void	raycast(t_game *game);
void	cast_ray(t_game *game, int x);
void	init_ray(t_game *game, t_ray *ray, int x);
void	calculate_step(t_game *game, t_ray *ray);
void	perform_dda(t_game *game, t_ray *ray);
void	calculate_wall_distance(t_game *game, t_ray *ray);
void	calculate_wall_height(t_ray *ray);
void	calculate_texture_coords(t_game *game, t_ray *ray);
int		get_texture_index(t_ray *ray);

void	handle_input(t_game *game);
int		is_valid_position(t_game *game, double x, double y);
void	move_forward(t_game *game, double move_speed);
void	move_backward(t_game *game, double move_speed);
void	strafe_left(t_game *game, double move_speed);
void	strafe_right(t_game *game, double move_speed);
void	rotate_left(t_game *game, double rot_speed);
void	rotate_right(t_game *game, double rot_speed);
t_ui32	get_rgba(int r, int g, int b, int a);
void	draw_pixel(t_ray *ray, t_game *game, int x, int y);

#endif