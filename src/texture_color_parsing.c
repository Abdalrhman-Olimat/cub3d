#include "../include/cub3d.h"

// Helper function to skip whitespace
char *skip_whitespace(char *str)
{
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    return (str);
}

// Helper function to trim trailing whitespace
void trim_trailing_whitespace(char *str)
{
    int len = ft_strlen(str);
    
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t'))
    {
        str[len - 1] = '\0';
        len--;
    }
}

// Parse texture line (NO, SO, WE, EA)
int parse_texture(char *line, t_texture *texture, char *identifier)
{
    char *ptr;
    char *path_start;
    
    // Skip identifier
    ptr = line + ft_strlen(identifier);
    ptr = skip_whitespace(ptr);
    
    if (!*ptr)
    {
        printf(ERR_TEXTURE);
        return (0);
    }
    
    path_start = ptr;
    
    // Find end of path (before any trailing whitespace or comments)
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
        ptr++;
    *ptr = '\0';
    
    // Check if file exists and is readable
    if (access(path_start, R_OK) != 0)
    {
        printf("Error\nTexture file not accessible: %s\n", path_start);
        return (0);
    }
    
    // Check if it's a regular file (not a directory)
    struct stat file_stat;
    if (stat(path_start, &file_stat) != 0 || !S_ISREG(file_stat.st_mode))
    {
        printf("Error\nTexture path is not a regular file: %s\n", path_start);
        return (0);
    }
    
    // Check if file has a proper extension (optional strict check)
    char *extension = ft_strrchr(path_start, '.');
    if (!extension || (ft_strncmp(extension, ".xpm", 4) != 0 && 
                       ft_strncmp(extension, ".png", 4) != 0 && 
                       ft_strncmp(extension, ".jpg", 4) != 0 && 
                       ft_strncmp(extension, ".jpeg", 5) != 0))
    {
        printf("Error\nTexture file must have valid extension (.xpm, .png, .jpg, .jpeg): %s\n", path_start);
        return (0);
    }
    
    texture->path = ft_strdup(path_start);
    if (!texture->path)
    {
        printf(ERR_MALLOC);
        return (0);
    }
    
    return (1);
}

// Validate RGB values (0-255 range)
int validate_rgb(int r, int g, int b)
{
    return (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255);
}

// Parse color line (F or C)
int parse_color(char *line, t_color *color, char identifier)
{
    char *ptr;
    char *rgb_start;
    char **rgb_split;
    int r, g, b;
    
    // Skip identifier
    ptr = line + 1;
    ptr = skip_whitespace(ptr);
    
    if (!*ptr)
    {
        printf(ERR_COLOR);
        return (0);
    }
    
    rgb_start = ptr;
    trim_trailing_whitespace(rgb_start);
    
    // Split by comma
    rgb_split = ft_split(rgb_start, ',');
    if (!rgb_split || !rgb_split[0] || !rgb_split[1] || !rgb_split[2] || rgb_split[3])
    {
        free_array(rgb_split);
        printf("Error\nColor must have exactly 3 RGB values\n");
        return (0);
    }
    
    // Parse RGB values
    r = ft_atoi(rgb_split[0]);
    g = ft_atoi(rgb_split[1]);
    b = ft_atoi(rgb_split[2]);
    
    free_array(rgb_split);
    
    if (!validate_rgb(r, g, b))
    {
        printf("Error\nRGB values must be in range [0,255]\n");
        return (0);
    }
    
    color->r = r;
    color->g = g;
    color->b = b;
    
    return (1);
}

// Phase 2C: Element Parsing (textures and colors)
int parse_textures_and_colors(t_game *game)
{
    int i;
    char *line;
    int texture_count;
    int floor_set;
    int ceiling_set;
    
    texture_count = 0;
    floor_set = 0;
    ceiling_set = 0;
    
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
        
        // Parse North texture
        if (ft_strncmp(line, "NO ", 3) == 0)
        {
            if (!parse_texture(line, &game->textures[0], "NO"))
                return (0);
            texture_count++;
        }
        // Parse South texture
        else if (ft_strncmp(line, "SO ", 3) == 0)
        {
            if (!parse_texture(line, &game->textures[1], "SO"))
                return (0);
            texture_count++;
        }
        // Parse West texture
        else if (ft_strncmp(line, "WE ", 3) == 0)
        {
            if (!parse_texture(line, &game->textures[2], "WE"))
                return (0);
            texture_count++;
        }
        // Parse East texture
        else if (ft_strncmp(line, "EA ", 3) == 0)
        {
            if (!parse_texture(line, &game->textures[3], "EA"))
                return (0);
            texture_count++;
        }
        // Parse Floor color
        else if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
        {
            if (!parse_color(line, &game->floor_color, 'F'))
                return (0);
            floor_set = 1;
        }
        // Parse Ceiling color
        else if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
        {
            if (!parse_color(line, &game->ceiling_color, 'C'))
                return (0);
            ceiling_set = 1;
        }
        // If we hit a line that starts with '1' or '0', we've reached the map
        else if (line[0] == '1' || line[0] == '0')
        {
            break;
        }
        
        i++;
    }
    
    // Check if all textures and colors are set
    if (texture_count != 4)
    {
        printf("Error\nMissing texture definitions (need NO, SO, WE, EA)\n");
        return (0);
    }
    
    if (!floor_set)
    {
        printf("Error\nMissing floor color definition (F)\n");
        return (0);
    }
    
    if (!ceiling_set)
    {
        printf("Error\nMissing ceiling color definition (C)\n");
        return (0);
    }
    
    return (1);
}
