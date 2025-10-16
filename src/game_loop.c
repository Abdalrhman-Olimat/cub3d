#include "../include/cub3d.h"

/**
 * Hook function called every frame
 */
void	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	
	// Handle ESC key to close window
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	
	// Handle player input (movement and rotation)
	handle_input(game);
	
	// Render the frame
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
 */
void	start_game_loop(t_game *game)
{
	// Register the loop hook
	mlx_loop_hook(game->mlx, &game_loop, game);
	
	// Start the MLX loop
	mlx_loop(game->mlx);
}

/**
 * Clean up graphics resources
 */
void	cleanup_graphics(t_game *game)
{
	int	i;

	// Delete textures
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].texture)
			mlx_delete_texture(game->textures[i].texture);
		i++;
	}

	// Delete image and terminate MLX
	if (game->img)
		mlx_delete_image(game->mlx, game->img);
	if (game->mlx)
		mlx_terminate(game->mlx);
}
