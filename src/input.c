/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:26:10 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/07 15:46:05 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/**
 * Check if the new position collides with a wall
 * Returns 1 if position is valid (no collision), 0 if collision
 * Check bounds
 * Check if position is a wall
 */
int	is_valid_position(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_x >= game->map.width
		|| map_y < 0 || map_y >= game->map.height)
		return (0);
	if (game->map.grid[map_y][map_x] == WALL)
		return (0);
	return (1);
}

/**
 * Strafe left (perpendicular to direction)
 * Use camera plane for perpendicular movement
 * Check for collisions FOR X and Y separately
 */
void	strafe_left(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.plane_x * move_speed;
	new_y = game->player.y - game->player.plane_y * move_speed;
	if (is_valid_position(game, new_x, game->player.y))
		game->player.x = new_x;
	if (is_valid_position(game, game->player.x, new_y))
		game->player.y = new_y;
}

/**
 * Strafe right (perpendicular to direction)
 * Use camera plane for perpendicular movement
 * Check for collisions FOR X and Y separately
 */
void	strafe_right(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.plane_x * move_speed;
	new_y = game->player.y + game->player.plane_y * move_speed;
	if (is_valid_position(game, new_x, game->player.y))
		game->player.x = new_x;
	if (is_valid_position(game, game->player.x, new_y))
		game->player.y = new_y;
}

/**
 * Handle all player input
 * Check key states and call movement/rotation functions
 */
void	handle_input(t_game *game)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.05;
	rot_speed = 0.03;
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_forward(game, move_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_backward(game, move_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		strafe_left(game, move_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		strafe_right(game, move_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_right(game, rot_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_left(game, rot_speed);
}
