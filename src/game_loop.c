/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeleimat <aeleimat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:19:41 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/05 14:19:42 by aeleimat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/**
 * Hook function called every frame
 * Handle ESC key to close window
 * Handle player input (movement and rotation)
 * Render the frame
 */
void	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	handle_input(game);
	render_frame(game);
}

/**
 * Hook function called when window is closed
 */

void	close_hook(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	mlx_close_window(game->mlx);
}

/**
 * Start the game loop
 * Register the loop hook
 * Start the MLX loop
 */
void	start_game_loop(t_game *game)
{
	mlx_loop_hook(game->mlx, &game_loop, game);
	mlx_loop(game->mlx);
}

/**
 * Clean up graphics resources
 * Delete textures, image, and terminate MLX
 */
void	cleanup_graphics(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->textures[i].texture)
			mlx_delete_texture(game->textures[i].texture);
		i++;
	}
	if (game->img)
		mlx_delete_image(game->mlx, game->img);
	if (game->mlx)
		mlx_terminate(game->mlx);
}
