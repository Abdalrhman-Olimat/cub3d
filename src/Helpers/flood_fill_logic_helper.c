/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill_logic_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:45:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 11:45:56 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	is_boundary_position(t_game *game, int x, int y)
{
	return (x == 0 || x == game->map.width - 1 || y == 0
		|| y == game->map.height - 1);
}

int	flood_fill_recursive_calls(t_game *game, char **visited, int x, int y)
{
	return (flood_fill_check(game, visited, x + 1, y)
		&& flood_fill_check(game, visited, x - 1, y)
		&& flood_fill_check(game, visited, x, y + 1)
		&& flood_fill_check(game, visited, x, y - 1));
}

int	handle_space_cell(t_game *game, char **visited, int x, int y)
{
	if (is_boundary_position(game, x, y))
		return (0);
	return (flood_fill_recursive_calls(game, visited, x, y));
}

int	handle_empty_cell(t_game *game, char **visited, int x, int y)
{
	if (is_boundary_position(game, x, y))
		return (0);
	return (flood_fill_recursive_calls(game, visited, x, y));
}

int	flood_fill_check(t_game *game, char **visited, int x, int y)
{
	if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
		return (0);
	if (visited[y][x] || game->map.grid[y][x] == WALL)
		return (1);
	visited[y][x] = 1;
	if (game->map.grid[y][x] == ' ')
		return (handle_space_cell(game, visited, x, y));
	if (game->map.grid[y][x] == EMPTY)
		return (handle_empty_cell(game, visited, x, y));
	return (1);
}
