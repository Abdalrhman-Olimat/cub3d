/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:40:01 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/07 11:48:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	validate_and_setup_map(t_game *game, int *map_start)
{
	*map_start = find_map_start(game);
	if (*map_start == -1)
	{
		printf("Error\nNo map found in file\n");
		return (0);
	}
	calculate_map_dimensions(game, *map_start);
	if (game->map.height == 0 || game->map.width == 0)
	{
		printf("Error\nEmpty map\n");
		return (0);
	}
	return (1);
}

int	parse_map(t_game *game)
{
	int	map_start;

	if (!validate_and_setup_map(game, &map_start))
		return (0);
	if (!allocate_map_grid(game))
		return (0);
	if (!populate_map_grid(game, map_start))
		return (0);
	return (1);
}
