/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_check_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:45:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 11:57:38 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	**allocate_visited_array(t_game *game)
{
	char	**visited;
	int		i;

	visited = malloc(sizeof(char *) * game->map.height);
	if (!visited)
		return (NULL);
	i = 0;
	while (i < game->map.height)
	{
		visited[i] = ft_calloc(game->map.width, sizeof(char));
		if (!visited[i])
		{
			while (--i >= 0)
				free(visited[i]);
			free(visited);
			return (NULL);
		}
		i++;
	}
	return (visited);
}

int	validate_all_visited_cells(t_game *game, char **visited)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (!validate_visited_cell(game, visited, i, j))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	cleanup_visited_array(char **visited, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(visited[i]);
		i++;
	}
	free(visited);
}

int	check_walls(t_game *game)
{
	char	**visited;

	visited = allocate_visited_array(game);
	if (!visited)
		return (0);
	mark_reachable_floors(game, visited, (int)game->player.x,
		(int)game->player.y);
	if (!validate_all_visited_cells(game, visited))
		return (0);
	cleanup_visited_array(visited, game->map.height);
	return (1);
}
