#include "../include/cub3d.h"

/**
 * Initialize ray direction and delta distances
 */
void	init_ray(t_game *game, t_ray *ray, int x)
{
	// Calculate ray position and direction
	ray->camera_x = 2 * x / (double)WIN_WIDTH - 1;
	ray->dir_x = game->player.dir_x + game->player.plane_x * ray->camera_x;
	ray->dir_y = game->player.dir_y + game->player.plane_y * ray->camera_x;
	
	// Current map position
	ray->map_x = (int)game->player.x;
	ray->map_y = (int)game->player.y;
	
	// Length of ray from one x or y-side to next x or y-side
	if (ray->dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1 / ray->dir_x);
	
	if (ray->dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1 / ray->dir_y);
	
	ray->hit = 0;
}

/**
 * Calculate step direction and initial side distances
 */
void	calculate_step(t_game *game, t_ray *ray)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player.x) * ray->delta_dist_x;
	}
	
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player.y) * ray->delta_dist_y;
	}
}

/**
 * Perform DDA algorithm to find wall intersection
 */
void	perform_dda(t_game *game, t_ray *ray)
{
	while (ray->hit == 0)
	{
		// Jump to next map square in x or y direction
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0; // X-side
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1; // Y-side
		}
		
		// Check if ray has hit a wall
		if (ray->map_x < 0 || ray->map_x >= game->map.width ||
			ray->map_y < 0 || ray->map_y >= game->map.height)
			ray->hit = 1;
		else if (game->map.grid[ray->map_y][ray->map_x] == WALL)
			ray->hit = 1;
	}
}

/**
 * Calculate perpendicular wall distance to avoid fisheye effect
 */
void	calculate_wall_distance(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player.x + 
			(1 - ray->step_x) / 2) / ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player.y + 
			(1 - ray->step_y) / 2) / ray->dir_y;
}

/**
 * Calculate wall line height and draw positions
 */
void	calculate_wall_height(t_ray *ray)
{
	// Calculate height of line to draw on screen
	ray->line_height = (int)(WIN_HEIGHT / ray->perp_wall_dist);
	
	// Calculate lowest and highest pixel to fill in current stripe
	ray->draw_start = -ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	
	ray->draw_end = ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}

/**
 * Determine which texture to use based on wall side
 */
int	get_texture_index(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (3); // East texture (EA)
		else
			return (2); // West texture (WE)
	}
	else
	{
		if (ray->step_y > 0)
			return (1); // South texture (SO)
		else
			return (0); // North texture (NO)
	}
}

/**
 * Calculate texture coordinate
 */
void	calculate_texture_coords(t_game *game, t_ray *ray)
{
	double	wall_x;
	
	// Calculate where exactly the wall was hit
	if (ray->side == 0)
		wall_x = game->player.y + ray->perp_wall_dist * ray->dir_y;
	else
		wall_x = game->player.x + ray->perp_wall_dist * ray->dir_x;
	wall_x -= floor(wall_x);
	
	// X coordinate on the texture
	ray->tex_x = (int)(wall_x * (double)ray->texture->width);
	if (ray->side == 0 && ray->dir_x > 0)
		ray->tex_x = ray->texture->width - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		ray->tex_x = ray->texture->width - ray->tex_x - 1;
	
	// Calculate step for texture Y coordinate
	ray->step_tex = 1.0 * ray->texture->height / ray->line_height;
	ray->tex_pos = (ray->draw_start - WIN_HEIGHT / 2 + 
		ray->line_height / 2) * ray->step_tex;
}

/**
 * Cast a single ray and render the wall stripe
 */
void	cast_ray(t_game *game, int x)
{
	t_ray	ray;
	int		y;
	int		tex_y;
	uint32_t	color;
	uint8_t	*pixel;
	
	// Initialize and calculate ray
	init_ray(game, &ray, x);
	calculate_step(game, &ray);
	perform_dda(game, &ray);
	calculate_wall_distance(game, &ray);
	calculate_wall_height(&ray);
	
	// Get texture and calculate coordinates
	ray.texture = game->textures[get_texture_index(&ray)].texture;
	if (!ray.texture)
		return;
	
	calculate_texture_coords(game, &ray);
	
	// Draw the textured vertical line
	y = ray.draw_start;
	while (y < ray.draw_end)
	{
		tex_y = (int)ray.tex_pos & (ray.texture->height - 1);
		ray.tex_pos += ray.step_tex;
		
		// Get pixel color from texture
		pixel = &ray.texture->pixels[(tex_y * ray.texture->width + ray.tex_x) * 4];
		color = get_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
		
		// Apply darkness to Y-sides for better depth perception
		if (ray.side == 1)
			color = (color >> 1) & 0x7F7F7F7F;
		
		put_pixel(game->img, x, y, color);
		y++;
	}
}

/**
 * Main raycasting function - cast rays for all screen columns
 */
void	raycast(t_game *game)
{
	int	x;
	
	x = 0;
	while (x < WIN_WIDTH)
	{
		cast_ray(game, x);
		x++;
	}
}
