/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeleimat <aeleimat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:40:01 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/05 16:46:01 by aeleimat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int is_valid_map_char(char c)
{
    return (c == WALL || c == EMPTY || c == NORTH || c == SOUTH || 
            c == EAST || c == WEST || c == ' ');
}

int is_player_char(char c)
{
    return (c == NORTH || c == SOUTH || c == EAST || c == WEST);
}

int find_map_start(t_game *game)
{
    int i;
    char *line;
    
    i = 0;
    while (game->file_content[i])
    {
        line = skip_whitespace(game->file_content[i]);
        if (!*line)
        {
            i++;
            continue;
        }
        if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0 ||
            ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0 ||
            line[0] == 'F' || line[0] == 'C')
        {
            i++;
            continue;
        }
        if (line[0] == '1' || line[0] == '0' || line[0] == ' ')
            return (i);
        i++;
    }
    return (-1);
}

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

int parse_map(t_game *game)
{
    int map_start;
    int i, j;
    char *line;
    
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
    i = 0;
    while (i < game->map.height)
    {
        line = game->file_content[map_start + i];
        
        game->map.grid[i] = malloc(sizeof(char) * (game->map.width + 1));
        if (!game->map.grid[i])
        {
            printf(ERR_MALLOC);
            while (--i >= 0)
                free(game->map.grid[i]);
            free(game->map.grid);
            game->map.grid = NULL;
            return (0);
        }
        j = 0;
        while (j < game->map.width)
        {
            if (j < (int)ft_strlen(line))
            {
                if (!is_valid_map_char(line[j]))
                {
                    printf("Error\nInvalid character '%c' in map at line %d, column %d\n", 
                           line[j], i + 1, j + 1);
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

int is_wall_or_void(t_game *game, int x, int y)
{
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return (1);
    
    return (game->map.grid[y][x] == WALL || game->map.grid[y][x] == ' ');
}

int flood_fill_check(t_game *game, char **visited, int x, int y)
{
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return (0);
    if (visited[y][x] || game->map.grid[y][x] == WALL)
        return (1);
    visited[y][x] = 1;
    if (game->map.grid[y][x] == ' ')
    {
        if (x == 0 || x == game->map.width - 1 || y == 0
            || y == game->map.height - 1)
            return (0);
        return (flood_fill_check(game, visited, x + 1, y) &&
                flood_fill_check(game, visited, x - 1, y) &&
                flood_fill_check(game, visited, x, y + 1) &&
                flood_fill_check(game, visited, x, y - 1));
    }
    if (game->map.grid[y][x] == EMPTY)
    {
        if (x == 0 || x == game->map.width - 1 || y == 0
            || y == game->map.height - 1)
            return (0);
        return (flood_fill_check(game, visited, x + 1, y) &&
                flood_fill_check(game, visited, x - 1, y) &&
                flood_fill_check(game, visited, x, y + 1) &&
                flood_fill_check(game, visited, x, y - 1));
    }
    return (1);
}

void mark_reachable_floors(t_game *game, char **visited, int x, int y)
{
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return;
    if (visited[y][x])
        return;
    if (game->map.grid[y][x] != EMPTY)
        return;
    visited[y][x] = 1;
    mark_reachable_floors(game, visited, x + 1, y);
    mark_reachable_floors(game, visited, x - 1, y);
    mark_reachable_floors(game, visited, x, y + 1);
    mark_reachable_floors(game, visited, x, y - 1);
}
int has_adjacent_space(t_game *game, int x, int y)
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

void free_visited(char **visited, int height)
{
    int k;
    
    k = 0;
    while (k < height)
    {
        free(visited[k]);
        k++;
    }
    free(visited);
}

int validate_visited_cell(t_game *game, char **visited, int i, int j)
{
    if (!visited[i][j])
        return (1);
    if (i == 0 || i == game->map.height - 1 || 
        j == 0 || j == game->map.width - 1)
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

int check_walls(t_game *game)
{
    char **visited;
    int i, j;
    
    visited = malloc(sizeof(char *) * game->map.height);
    if (!visited)
        return (0);
    i = 0;
    while (i < game->map.height)
    {
        visited[i] = ft_calloc(game->map.width, sizeof(char));
        if (!visited[i])
        {
            while (--i >= 0)
                free(visited[i]);
            free(visited);
            return (0);
        }
        i++;
    }
    mark_reachable_floors(game, visited, (int)game->player.x, (int)game->player.y);
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
    i = 0;
    while (i < game->map.height)
    {
        free(visited[i]);
        i++;
    }
    free(visited);
    
    return (1);
}

int validate_map(t_game *game)
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
