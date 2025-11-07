/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_texture_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:20:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 15:53:23 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	calculate_texture_coords(t_game *game, t_ray *ray)
{
	double	wall_x;
	int		texture_index;

	texture_index = get_texture_index(ray);
	ray->texture = game->textures[texture_index].texture;
	if (ray->side == 0)
		wall_x = game->player.y + ray->perp_wall_dist * ray->dir_y;
	else
		wall_x = game->player.x + ray->perp_wall_dist * ray->dir_x;
	wall_x -= floor(wall_x);
	ray->tex_x = (int)(wall_x * (double)ray->texture->width);
	if (ray->side == 0 && ray->dir_x < 0)
		ray->tex_x = ray->texture->width - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir_y > 0)
		ray->tex_x = ray->texture->width - ray->tex_x - 1;
	ray->step_tex = 1.0 * ray->texture->height / ray->line_height;
	ray->tex_pos = (ray->draw_start - WIN_HEIGHT / 2
			+ ray->line_height / 2) * ray->step_tex;
}
