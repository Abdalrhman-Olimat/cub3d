/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid_alloc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:00:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 15:29:17 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	free_grid_rows(t_game *game, int row_count)
{
	int	i;

	i = 0;
	while (i < row_count)
	{
		free(game->map.grid[i]);
		i++;
	}
	free(game->map.grid);
	game->map.grid = NULL;
}

static int	validate_char_and_set(t_game *game, char *line, int i, int j)
{
	if (!is_valid_map_char(line[j]))
	{
		printf("Error\nInvalid character '%c' in map\n", line[j]);
		free(game->map.grid[i]);
		free_grid_rows(game, i);
		return (0);
	}
	game->map.grid[i][j] = line[j];
	return (1);
}

static int	fill_row(t_game *game, char *line, int i)
{
	int	j;
	int	line_len;

	j = 0;
	line_len = ft_strlen(line);
	while (j < game->map.width)
	{
		if (j < line_len)
		{
			if (!validate_char_and_set(game, line, i, j))
				return (0);
		}
		else
			game->map.grid[i][j] = ' ';
		j++;
	}
	game->map.grid[i][game->map.width] = '\0';
	return (1);
}

int	allocate_and_fill_grid(t_game *game, int map_start)
{
	int	i;

	i = 0;
	while (i < game->map.height)
	{
		game->map.grid[i] = malloc(sizeof(char) * (game->map.width + 1));
		if (!game->map.grid[i])
		{
			printf(ERR_MALLOC);
			free_grid_rows(game, i);
			return (0);
		}
		if (!fill_row(game, game->file_content[map_start + i], i))
			return (0);
		i++;
	}
	game->map.grid[game->map.height] = NULL;
	return (1);
}
