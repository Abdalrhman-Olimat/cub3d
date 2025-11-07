/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:45:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 11:48:35 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	set_player_data(t_game *game, int j, int i, char orientation)
{
	game->player.x = (double)j + 0.5;
	game->player.y = (double)i + 0.5;
	game->player.orientation = orientation;
	game->map.grid[i][j] = EMPTY;
}

int	validate_player_count(int player_count)
{
	if (player_count == 0)
	{
		printf("Error\nNo player starting position found\n");
		return (0);
	}
	if (player_count > 1)
	{
		printf("Error\nMultiple player starting positions found\n");
		return (0);
	}
	return (1);
}

int	find_player(t_game *game)
{
	int	i;
	int	j;
	int	player_count;

	player_count = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (is_player_char(game->map.grid[i][j]))
			{
				set_player_data(game, j, i, game->map.grid[i][j]);
				player_count++;
			}
			j++;
		}
		i++;
	}
	return (validate_player_count(player_count));
}

void	set_north_orientation(t_game *game)
{
	game->player.dir_x = 0;
	game->player.dir_y = -1;
	game->player.plane_x = 0.66;
	game->player.plane_y = 0;
}

void	set_south_orientation(t_game *game)
{
	game->player.dir_x = 0;
	game->player.dir_y = 1;
	game->player.plane_x = -0.66;
	game->player.plane_y = 0;
}
