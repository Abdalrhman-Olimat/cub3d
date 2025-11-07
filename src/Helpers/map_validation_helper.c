/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:30:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 11:55:31 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	is_wall_or_void(t_game *game, int x, int y)
{
	if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
		return (1);
	return (game->map.grid[y][x] == WALL || game->map.grid[y][x] == ' ');
}

int	has_adjacent_space(t_game *game, int x, int y)
{
	if (x + 1 < game->map.width && game->map.grid[y][x + 1] == ' ')
		return (1);
	if (x - 1 >= 0 && game->map.grid[y][x - 1] == ' ')
		return (1);
	if (y + 1 < game->map.height && game->map.grid[y + 1][x] == ' ')
		return (1);
	if (y - 1 >= 0 && game->map.grid[y - 1][x] == ' ')
		return (1);
	return (0);
}

void	free_visited(char **visited, int height)
{
	int	k;

	k = 0;
	while (k < height)
	{
		free(visited[k]);
		k++;
	}
	free(visited);
}

int	validate_visited_cell(t_game *game, char **visited, int i, int j)
{
	if (!visited[i][j])
		return (1);
	if (i == 0 || i == game->map.height - 1
		|| j == 0 || j == game->map.width - 1)
	{
		free_visited(visited, game->map.height);
		return (0);
	}
	if (has_adjacent_space(game, j, i))
	{
		free_visited(visited, game->map.height);
		return (0);
	}
	return (1);
}

void	mark_reachable_floors(t_game *game, char **visited, int x, int y)
{
	if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
		return ;
	if (visited[y][x])
		return ;
	if (game->map.grid[y][x] != EMPTY)
		return ;
	visited[y][x] = 1;
	mark_reachable_floors(game, visited, x + 1, y);
	mark_reachable_floors(game, visited, x - 1, y);
	mark_reachable_floors(game, visited, x, y + 1);
	mark_reachable_floors(game, visited, x, y - 1);
}
