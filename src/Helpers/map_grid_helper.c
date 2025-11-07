/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:26:10 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/07 11:47:02 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	allocate_map_grid(t_game *game)
{
	game->map.grid = malloc(sizeof(char *) * (game->map.height + 1));
	if (!game->map.grid)
	{
		printf(ERR_MALLOC);
		return (0);
	}
	return (1);
}

void	cleanup_grid_on_error(t_game *game, int current_row)
{
	while (--current_row >= 0)
		free(game->map.grid[current_row]);
	free(game->map.grid);
	game->map.grid = NULL;
}

int	validate_and_set_char(t_game *game, char *line, int i, int j)
{
	if (!is_valid_map_char(line[j]))
	{
		printf("Error\nInvalid character '%c' in map at "
			"line %d, column %d\n", line[j], i + 1, j + 1);
		free(game->map.grid[i]);
		cleanup_grid_on_error(game, i);
		return (0);
	}
	game->map.grid[i][j] = line[j];
	return (1);
}

int	populate_row_chars(t_game *game, char *line, int i)
{
	int	j;

	j = 0;
	while (j < game->map.width)
	{
		if (j < (int)ft_strlen(line))
		{
			if (!validate_and_set_char(game, line, i, j))
				return (0);
		}
		else
		{
			game->map.grid[i][j] = ' ';
		}
		j++;
	}
	game->map.grid[i][game->map.width] = '\0';
	return (1);
}

int	populate_map_grid(t_game *game, int map_start)
{
	int		i;
	char	*line;

	i = 0;
	while (i < game->map.height)
	{
		line = game->file_content[map_start + i];
		game->map.grid[i] = malloc(sizeof(char) * (game->map.width + 1));
		if (!game->map.grid[i])
		{
			printf(ERR_MALLOC);
			cleanup_grid_on_error(game, i);
			return (0);
		}
		if (!populate_row_chars(game, line, i))
			return (0);
		i++;
	}
	game->map.grid[game->map.height] = NULL;
	return (1);
}
