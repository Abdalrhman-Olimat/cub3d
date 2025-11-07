/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_floor_ceiling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:00:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:35:29 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	render_ceiling(t_game *game, uint32_t ceiling_color)
{
	int	x;
	int	y;

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
}

static void	render_floor(t_game *game, uint32_t floor_color)
{
	int	x;
	int	y;

	y = WIN_HEIGHT / 2;
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

void	render_floor_ceiling(t_game *game)
{
	uint32_t	floor_color;
	uint32_t	ceiling_color;

	floor_color = get_rgba(game->floor_color.r, game->floor_color.g,
			game->floor_color.b, 255);
	ceiling_color = get_rgba(game->ceiling_color.r, game->ceiling_color.g,
			game->ceiling_color.b, 255);
	render_ceiling(game, ceiling_color);
	render_floor(game, floor_color);
}
