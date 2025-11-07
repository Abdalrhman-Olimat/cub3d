/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:15:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 15:17:31 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	calculate_wall_distance(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player.x
				+ (1 - ray->step_x) / 2) / ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player.y
				+ (1 - ray->step_y) / 2) / ray->dir_y;
}

void	calculate_wall_height(t_ray *ray)
{
	ray->line_height = (int)(WIN_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}

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
