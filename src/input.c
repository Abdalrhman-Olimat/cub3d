#include "../include/cub3d.h"

/**
 * Check if the new position collides with a wall
 * Returns 1 if position is valid (no collision), 0 if collision
 */
int	is_valid_position(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;
	
	map_x = (int)x;
	map_y = (int)y;
	
	// Check bounds
	if (map_x < 0 || map_x >= game->map.width ||
		map_y < 0 || map_y >= game->map.height)
		return (0);
	
	// Check if position is a wall
	if (game->map.grid[map_y][map_x] == WALL)
		return (0);
	
	return (1);
}

/**
 * Move player forward in the direction they're facing
 */
void	move_forward(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;
	
	new_x = game->player.x + game->player.dir_x * move_speed;
	new_y = game->player.y + game->player.dir_y * move_speed;
	
	// Check collision for X movement
	if (is_valid_position(game, new_x, game->player.y))
		game->player.x = new_x;
	
	// Check collision for Y movement
	if (is_valid_position(game, game->player.x, new_y))
		game->player.y = new_y;
}

/**
 * Move player backward (opposite of forward direction)
 */
void	move_backward(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;
	
	new_x = game->player.x - game->player.dir_x * move_speed;
	new_y = game->player.y - game->player.dir_y * move_speed;
	
	// Check collision for X movement
	if (is_valid_position(game, new_x, game->player.y))
		game->player.x = new_x;
	
	// Check collision for Y movement
	if (is_valid_position(game, game->player.x, new_y))
		game->player.y = new_y;
}

/**
 * Strafe left (perpendicular to direction)
 */
void	strafe_left(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;
	
	// Use camera plane for perpendicular movement
	new_x = game->player.x - game->player.plane_x * move_speed;
	new_y = game->player.y - game->player.plane_y * move_speed;
	
	// Check collision
	if (is_valid_position(game, new_x, game->player.y))
		game->player.x = new_x;
	
	if (is_valid_position(game, game->player.x, new_y))
		game->player.y = new_y;
}

/**
 * Strafe right (perpendicular to direction)
 */
void	strafe_right(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;
	
	// Use camera plane for perpendicular movement
	new_x = game->player.x + game->player.plane_x * move_speed;
	new_y = game->player.y + game->player.plane_y * move_speed;
	
	// Check collision
	if (is_valid_position(game, new_x, game->player.y))
		game->player.x = new_x;
	
	if (is_valid_position(game, game->player.x, new_y))
		game->player.y = new_y;
}

/**
 * Rotate player view left
 */
void	rotate_left(t_game *game, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;
	
	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	
	// Rotate direction vector
	game->player.dir_x = game->player.dir_x * cos(rot_speed) - 
		game->player.dir_y * sin(rot_speed);
	game->player.dir_y = old_dir_x * sin(rot_speed) + 
		game->player.dir_y * cos(rot_speed);
	
	// Rotate camera plane
	game->player.plane_x = game->player.plane_x * cos(rot_speed) - 
		game->player.plane_y * sin(rot_speed);
	game->player.plane_y = old_plane_x * sin(rot_speed) + 
		game->player.plane_y * cos(rot_speed);
}

/**
 * Rotate player view right
 */
void	rotate_right(t_game *game, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;
	
	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	
	// Rotate direction vector (negative angle)
	game->player.dir_x = game->player.dir_x * cos(-rot_speed) - 
		game->player.dir_y * sin(-rot_speed);
	game->player.dir_y = old_dir_x * sin(-rot_speed) + 
		game->player.dir_y * cos(-rot_speed);
	
	// Rotate camera plane (negative angle)
	game->player.plane_x = game->player.plane_x * cos(-rot_speed) - 
		game->player.plane_y * sin(-rot_speed);
	game->player.plane_y = old_plane_x * sin(-rot_speed) + 
		game->player.plane_y * cos(-rot_speed);
}

/**
 * Handle all player input
 */
void	handle_input(t_game *game)
{
	double	move_speed;
	double	rot_speed;
	
	move_speed = 0.05;  // Movement speed per frame
	rot_speed = 0.03;   // Rotation speed per frame
	
	// Movement keys (WASD)
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_forward(game, move_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_backward(game, move_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		strafe_left(game, move_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		strafe_right(game, move_speed);
	
	// Rotation keys (Arrow keys) - swapped to fix inverted rotation
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_right(game, rot_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_left(game, rot_speed);
	
	// Exit key (ESC) - handled in game_loop.c
}
