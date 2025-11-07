/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_orientation_helper.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:45:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:59:02 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
/*
 * These functions are now provided by src/map_parsing.c (the working
 * implementation was restored there). Wrap the old definitions so they
 * are not compiled and avoid duplicate symbol definitions.
 */

#if 0
void	set_east_orientation(t_game *game)
{
	game->player.dir_x = 1;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = 0.66;
}

void	set_west_orientation(t_game *game)
{
	game->player.dir_x = -1;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = -0.66;
}

void	init_player(t_game *game)
{
	if (game->player.orientation == NORTH)
		set_north_orientation(game);
	else if (game->player.orientation == SOUTH)
		set_south_orientation(game);
	else if (game->player.orientation == EAST)
		set_east_orientation(game);
	else if (game->player.orientation == WEST)
		set_west_orientation(game);
}

int	validate_map(t_game *game)
{
	if (!find_player(game))
		return (0);
	init_player(game);
	if (!check_walls(game))
	{
		printf("Error\nMap is not properly enclosed by walls\n");
		return (0);
	}
	return (1);
}
#endif
