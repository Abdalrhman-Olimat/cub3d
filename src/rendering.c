#include "../include/cub3d.h"

/**
 * Convert RGB values to MLX42 color format (RGBA)
 * MLX42 uses format: 0xRRGGBBAA
 */
uint32_t	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

/**
 * Put a pixel at position (x, y) with given color
 */
void	put_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
		mlx_put_pixel(img, x, y, color);
}

/**
 * Clear the entire image with a single color
 */
void	clear_image(mlx_image_t *img, uint32_t color)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

/**
 * Draw vertical line from y1 to y2 at position x with given color
 */
void	draw_vertical_line(mlx_image_t *img, int x, int y1, int y2, uint32_t color)
{
	int	y;

	if (y1 > y2)
	{
		y = y1;
		y1 = y2;
		y2 = y;
	}
	
	y = y1;
	while (y <= y2)
	{
		put_pixel(img, x, y, color);
		y++;
	}
}

/**
 * Render the floor and ceiling
 */
void	render_floor_ceiling(t_game *game)
{
	uint32_t	floor_color;
	uint32_t	ceiling_color;
	int		x;
	int		y;

	floor_color = get_rgba(game->floor_color.r, game->floor_color.g,
		game->floor_color.b, 255);
	ceiling_color = get_rgba(game->ceiling_color.r, game->ceiling_color.g,
		game->ceiling_color.b, 255);

	// Draw ceiling (top half)
	y = 0;
	while (y < WIN_HEIGHT / 2)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			mlx_put_pixel(game->img, x, y, ceiling_color);
			x++;
		}
		y++;
	}

	// Draw floor (bottom half)
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			mlx_put_pixel(game->img, x, y, floor_color);
			x++;
		}
		y++;
	}
}

/**
 * Main render function - called every frame
 */
void	render_frame(t_game *game)
{
	// Clear screen and draw floor/ceiling
	render_floor_ceiling(game);
	
	// Perform raycasting to draw walls
	raycast(game);
}
