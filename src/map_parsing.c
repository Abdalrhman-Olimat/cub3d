/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:40:01 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/07 15:29:16 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	parse_map(t_game *game)
{
	int	map_start;

	map_start = find_map_start(game);
	if (map_start == -1)
	{
		printf("Error\nNo map found in file\n");
		return (0);
	}
	calculate_map_dimensions(game, map_start);
	if (game->map.height == 0 || game->map.width == 0)
	{
		printf("Error\nEmpty map\n");
		return (0);
	}
	game->map.grid = malloc(sizeof(char *) * (game->map.height + 1));
	if (!game->map.grid)
	{
		printf(ERR_MALLOC);
		return (0);
	}
	if (!allocate_and_fill_grid(game, map_start))
		return (0);
	return (1);
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
