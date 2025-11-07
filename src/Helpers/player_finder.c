/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_finder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:00:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 15:29:17 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	set_player_position(t_game *game, int j, int i)
{
	game->player.x = (double)j + 0.5;
	game->player.y = (double)i + 0.5;
	game->player.orientation = game->map.grid[i][j];
	game->map.grid[i][j] = EMPTY;
}

static int	validate_player_count(int count)
{
	if (count == 0)
	{
		printf("Error\nNo player starting position found\n");
		return (0);
	}
	if (count > 1)
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
				set_player_position(game, j, i);
				player_count++;
			}
			j++;
		}
		i++;
	}
	return (validate_player_count(player_count));
}
