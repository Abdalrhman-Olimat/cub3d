/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:30:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/08 08:56:49 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_ray(t_game *game, t_ray *ray, int x)
{
	ray->camera_x = 2 * x / (double)WIN_WIDTH - 1;
	ray->dir_x = game->player.dir_x + game->player.plane_x * ray->camera_x;
	ray->dir_y = game->player.dir_y + game->player.plane_y * ray->camera_x;
	ray->map_x = (int)game->player.x;
	ray->map_y = (int)game->player.y;
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
		ray->side_dist_x = (ray->map_x + 1.0 - game->player.x)
			* ray->delta_dist_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player.y)
			* ray->delta_dist_y;
	}
}

void	perform_dda(t_game *game, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (game->map.grid[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

void	cast_ray(t_game *game, int x)
{
	t_ray		ray;
	int			y;
	int			tex_y;
	uint32_t	color;
	uint8_t		*pixel;

	init_ray(game, &ray, x);
	calculate_step(game, &ray);
	perform_dda(game, &ray);
	calculate_wall_distance(game, &ray);
	calculate_wall_height(&ray);
	ray.texture = game->textures[get_texture_index(&ray)].texture;
	if (!ray.texture)
		return ;
	calculate_texture_coords(game, &ray);
	y = ray.draw_start;
	while (y < ray.draw_end)
	{
		tex_y = (int)ray.tex_pos & (ray.texture->height - 1);
		ray.tex_pos += ray.step_tex;
		pixel = &ray.texture->pixels[(tex_y * ray.texture->width + ray.tex_x)
			* 4];
		color = get_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
		if (ray.side == 1)
			color = (color >> 1) & 0x7F7F7F7F;
		put_pixel(game->img, x, y, color);
		y++;
	}
}

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
