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

// Check if position is a wall or out of bounds
int is_wall_or_void(t_game *game, int x, int y)
{
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return (1);
    
    return (game->map.grid[y][x] == WALL || game->map.grid[y][x] == ' ');
}

// Flood fill to check if map is properly enclosed
// This checks if any reachable position from (x,y) touches a space at the boundary
int flood_fill_check(t_game *game, char **visited, int x, int y)
{
    // Out of bounds
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return (0);
    
    // Already visited or it's a wall
    if (visited[y][x] || game->map.grid[y][x] == WALL)
        return (1);
    
    // Mark current position as visited
    visited[y][x] = 1;
    
    // If current position is a space character
    if (game->map.grid[y][x] == ' ')
    {
        // If this space is at the map boundary, the map is NOT properly enclosed
        if (x == 0 || x == game->map.width - 1 || y == 0 || y == game->map.height - 1)
            return (0);
        
        // Check all adjacent positions (recursively)
        return (flood_fill_check(game, visited, x + 1, y) &&
                flood_fill_check(game, visited, x - 1, y) &&
                flood_fill_check(game, visited, x, y + 1) &&
                flood_fill_check(game, visited, x, y - 1));
    }
    
    // If current position is empty floor (0)
    if (game->map.grid[y][x] == EMPTY)
    {
        // Empty floor cannot be at the boundary
        if (x == 0 || x == game->map.width - 1 || y == 0 || y == game->map.height - 1)
            return (0);
        
        // Check all 4 adjacent positions
        // Any adjacent space or empty floor must also be properly enclosed
        return (flood_fill_check(game, visited, x + 1, y) &&
                flood_fill_check(game, visited, x - 1, y) &&
                flood_fill_check(game, visited, x, y + 1) &&
                flood_fill_check(game, visited, x, y - 1));
    }
    
    return (1);
}

// Flood fill from player to mark all reachable empty floor tiles
void mark_reachable_floors(t_game *game, char **visited, int x, int y)
{
    // Out of bounds or already visited
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return;
    
    if (visited[y][x])
        return;
    
    // Only traverse empty floors
    if (game->map.grid[y][x] != EMPTY)
        return;
    
    // Mark as reachable
    visited[y][x] = 1;
    
    // Recursively check all 4 directions
    mark_reachable_floors(game, visited, x + 1, y);
    mark_reachable_floors(game, visited, x - 1, y);
    mark_reachable_floors(game, visited, x, y + 1);
    mark_reachable_floors(game, visited, x, y - 1);
}

// Check if a position has an adjacent space
int has_adjacent_space(t_game *game, int x, int y)
{
    // Check all 4 adjacent positions
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

// Check if map is surrounded by walls
int check_walls(t_game *game)
{
    char **visited;
    int i, j;
    
    // Create visited array to track reachable floors
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
    
    // Mark all empty floors reachable from player position
    mark_reachable_floors(game, visited, (int)game->player.x, (int)game->player.y);
    
    // Check if any reachable empty floor is adjacent to a space or at the boundary
    i = 0;
    while (i < game->map.height)
    {
        j = 0;
        while (j < game->map.width)
        {
            if (visited[i][j])
            {
                // If this floor is at the map boundary, it's invalid
                if (i == 0 || i == game->map.height - 1 || 
                    j == 0 || j == game->map.width - 1)
                {
                    // Free visited array
                    int k = 0;
                    while (k < game->map.height)
                    {
                        free(visited[k]);
                        k++;
                    }
                    free(visited);
                    return (0);
                }
                
                // If this floor is adjacent to a space, map is invalid
                if (has_adjacent_space(game, j, i))
                {
                    // Free visited array
                    int k = 0;
                    while (k < game->map.height)
                    {
                        free(visited[k]);
                        k++;
                    }
                    free(visited);
                    return (0);
                }
            }
            j++;
        }
        i++;
    }
    
    // Free visited array
    i = 0;
    while (i < game->map.height)
    {
        free(visited[i]);
        i++;
    }
    free(visited);
    
    return (1);
}

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
