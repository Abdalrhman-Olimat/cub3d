#include "../include/cub3d.h"

/**
 * Initialize the MLX42 library and create window
 * Returns 1 on success, 0 on failure
 */
int	init_mlx(t_game *game)
{
	game->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3D", true);
	if (!game->mlx)
	{
		printf("Error\nFailed to initialize MLX42\n");
		return (0);
	}
	return (1);
}

/**
 * Create the main image buffer for rendering
 * Returns 1 on success, 0 on failure
 */
int	init_image(t_game *game)
{
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
	{
		printf("Error\nFailed to create image buffer\n");
		mlx_terminate(game->mlx);
		return (0);
	}
	
	// Display the image in the window
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) == -1)
	{
		printf("Error\nFailed to display image\n");
		mlx_delete_image(game->mlx, game->img);
		mlx_terminate(game->mlx);
		return (0);
	}
	
	return (1);
}

/**
 * Load a single texture from file path
 * Returns 1 on success, 0 on failure
 */
int	load_texture(t_game *game, t_texture *tex)
{
	tex->texture = mlx_load_png(tex->path);
	if (!tex->texture)
	{
		printf("Error\nFailed to load texture: %s\n", tex->path);
		return (0);
	}
	return (1);
}

/**
 * Load all four wall textures
 * Returns 1 on success, 0 on failure
 */
int	load_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!load_texture(game, &game->textures[i]))
		{
			// Clean up already loaded textures
			while (--i >= 0)
			{
				if (game->textures[i].texture)
					mlx_delete_texture(game->textures[i].texture);
			}
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * Initialize all graphics components
 * Returns 1 on success, 0 on failure
 */
int	init_graphics(t_game *game)
{
	if (!init_mlx(game))
		return (0);
	
	if (!init_image(game))
		return (0);
	
	if (!load_textures(game))
	{
		mlx_delete_image(game->mlx, game->img);
		mlx_terminate(game->mlx);
		return (0);
	}
	
	return (1);
}
