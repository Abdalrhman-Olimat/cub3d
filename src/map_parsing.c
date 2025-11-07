/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:40:01 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/07 15:04:12 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
// Check if character is a valid map character
int is_valid_map_char(char c)
{
	return (c == WALL || c == EMPTY || c == NORTH || c == SOUTH || 
			c == EAST || c == WEST || c == ' ');
}

// Check if character is a player starting position
int is_player_char(char c)
{
	return (c == NORTH || c == SOUTH || c == EAST || c == WEST);
}

// Find the start of the map in file content
int find_map_start(t_game *game)
{
	int i;
	char *line;
    
	i = 0;
	while (game->file_content[i])
	{
		line = skip_whitespace(game->file_content[i]);
        
		// Skip empty lines
		if (!*line)
		{
			i++;
			continue;
		}
        
		// Skip texture and color lines
		if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0 ||
			ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0 ||
			line[0] == 'F' || line[0] == 'C')
		{
			i++;
			continue;
		}
        
		// If we hit a line that could be part of the map
		if (line[0] == '1' || line[0] == '0' || line[0] == ' ')
		{
			return (i);
		}
        
		i++;
	}
    
	return (-1);
}

// Calculate map dimensions
void calculate_map_dimensions(t_game *game, int map_start)
{
	int i;
	int max_width;
	int current_width;
    
	max_width = 0;
	game->map.height = 0;
    
	i = map_start;
	while (game->file_content[i])
	{
		current_width = ft_strlen(game->file_content[i]);
		if (current_width > max_width)
			max_width = current_width;
		game->map.height++;
		i++;
	}
    
	game->map.width = max_width;
}

// Phase 2D: Map Parsing
int parse_map(t_game *game)
{
	int map_start;
	int i, j;
	char *line;
    
	// Find where the map starts
	map_start = find_map_start(game);
	if (map_start == -1)
	{
		printf("Error\nNo map found in file\n");
		return (0);
	}
    
	// Calculate map dimensions
	calculate_map_dimensions(game, map_start);
    
	if (game->map.height == 0 || game->map.width == 0)
	{
		printf("Error\nEmpty map\n");
		return (0);
	}
    
	// Allocate map grid
	game->map.grid = malloc(sizeof(char *) * (game->map.height + 1));
	if (!game->map.grid)
	{
		printf(ERR_MALLOC);
		return (0);
	}
    
	// Copy map lines and validate characters
	i = 0;
	while (i < game->map.height)
	{
		line = game->file_content[map_start + i];
        
		// Allocate row with padding to map width
		game->map.grid[i] = malloc(sizeof(char) * (game->map.width + 1));
		if (!game->map.grid[i])
		{
			printf(ERR_MALLOC);
			// Free previously allocated rows
			while (--i >= 0)
				free(game->map.grid[i]);
			free(game->map.grid);
			game->map.grid = NULL;
			return (0);
		}
        
		// Copy characters and pad with spaces if needed
		j = 0;
		while (j < game->map.width)
		{
			if (j < (int)ft_strlen(line))
			{
				if (!is_valid_map_char(line[j]))
				{
					printf("Error\nInvalid character '%c' in map at line %d, column %d\n", 
						   line[j], i + 1, j + 1);
					// Free current row and all previous rows
					free(game->map.grid[i]);
					while (--i >= 0)
						free(game->map.grid[i]);
					free(game->map.grid);
					game->map.grid = NULL;
					return (0);
				}
				game->map.grid[i][j] = line[j];
			}
			else
			{
				game->map.grid[i][j] = ' ';
			}
			j++;
		}
		game->map.grid[i][game->map.width] = '\0';
		i++;
	}
	game->map.grid[game->map.height] = NULL;
    
	return (1);
}

// Find player position and set orientation
int find_player(t_game *game)
{
	int i, j;
	int player_count;
    
	player_count = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (is_player_char(game->map.grid[i][j]))
			{
				game->player.x = (double)j + 0.5;
				game->player.y = (double)i + 0.5;
				game->player.orientation = game->map.grid[i][j];
                
				// Replace player character with empty space
				game->map.grid[i][j] = EMPTY;
                
				player_count++;
			}
			j++;
		}
		i++;
	}
    
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

// Initialize player direction and camera plane based on orientation
void init_player(t_game *game)
{
	if (game->player.orientation == NORTH)
	{
		game->player.dir_x = 0;
		game->player.dir_y = -1;
		game->player.plane_x = 0.66;
		game->player.plane_y = 0;
	}
	else if (game->player.orientation == SOUTH)
	{
		game->player.dir_x = 0;
		game->player.dir_y = 1;
		game->player.plane_x = -0.66;
		game->player.plane_y = 0;
	}
	else if (game->player.orientation == EAST)
	{
		game->player.dir_x = 1;
		game->player.dir_y = 0;
		game->player.plane_x = 0;
		game->player.plane_y = 0.66;
	}
	else if (game->player.orientation == WEST)
	{
		game->player.dir_x = -1;
		game->player.dir_y = 0;
		game->player.plane_x = 0;
		game->player.plane_y = -0.66;
	}
}
// Note: wall-check and flood-fill helper functions are implemented in
// src/Helpers/* to avoid duplication. validate_map below uses those
// helper implementations.

// Phase 2E: Map Validation
int validate_map(t_game *game)
{
	// Find and validate player position
	if (!find_player(game))
		return (0);
    
	// Initialize player direction vectors
	init_player(game);
    
	// Check if map is properly enclosed by walls
	if (!check_walls(game))
	{
		printf("Error\nMap is not properly enclosed by walls\n");
		return (0);
	}
    
	return (1);
}
