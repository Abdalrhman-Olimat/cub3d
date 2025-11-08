/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeleimat <aeleimat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 17:23:55 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/08 17:24:42 by aeleimat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_pixel(t_ray *ray, t_game *game, int x, int y)
{
	int			tex_y;
	uint8_t		*pixel;
	uint32_t	color;

	tex_y = (int)ray->tex_pos & (ray->texture->height - 1);
	ray->tex_pos += ray->step_tex;
	pixel = &ray->texture->pixels[(tex_y * ray->texture->width + ray->tex_x)
		* 4];
	color = get_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
	if (ray->side == 1)
		color = (color >> 1) & 0x7F7F7F7F;
	put_pixel(game->img, x, y, color);
}
