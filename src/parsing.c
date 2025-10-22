#include "../include/cub3d.h"

// Phase 2A: Command Line Arguments validation
int parse_arguments(int argc, char **argv)
{
    char *extension;
    
    if (argc != 2)
    {
        printf(ERR_ARGC);
        return (0);
    }
    
    // Check .cub file extension
    extension = ft_strrchr(argv[1], '.');
    if (!extension || ft_strncmp(extension, ".cub", 5) != 0) // <--- this should be 5 to check for NULL termination
    {
        printf(ERR_FILE_EXT);
        return (0);
    }
    
    return (1);
}

// Phase 2B: File Reading & Basic Parsing
char **read_file(char *filename)
{
    int fd;
    char *line;
    char **content;
    char **temp;
    int line_count;
    int i;
    
    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        printf(ERR_FILE_OPEN);
        return (NULL);
    }
    
    content = NULL;
    line_count = 0;
    
    // Read file line by line
    while ((line = get_next_line(fd)) != NULL)
    {
        // Allocate/reallocate array for new line
        temp = malloc(sizeof(char *) * (line_count + 2));
        if (!temp)
        {
            free_array(content);
            free(line);
            close(fd);
            printf(ERR_MALLOC);
            return (NULL);
        }
        
        // Copy existing lines
        i = 0;
        while (i < line_count && content)
        {
            temp[i] = content[i];
            i++;
        }
        
        // Add new line (remove newline character)
        if (line[ft_strlen(line) - 1] == '\n')
            line[ft_strlen(line) - 1] = '\0';
        temp[line_count] = line;
        temp[line_count + 1] = NULL;
        
        free(content);
        content = temp;
        line_count++;
    }
    
    close(fd);
    
    // Handle empty file
    if (line_count == 0)
    {
        printf("Error\nEmpty file or file reading failed\n");
        return (NULL);
    }
    
    return (content);
}

int parse_file(char *filename, t_game *game)
{
    game->file_content = read_file(filename);
    if (!game->file_content)
        return (0);
    
    return (1);
}

// Utility function to free string array
void free_array(char **array)
{
    int i;
    
    if (!array)
        return;
    
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

// Clean up game resources
void cleanup_game(t_game *game)
{
    int i;
    
    if (!game)
        return;
    
    // Free file content
    if (game->file_content)
        free_array(game->file_content);
    
    // Free texture paths
    i = 0;
    while (i < 4)
    {
        if (game->textures[i].path)
        {
            free(game->textures[i].path);
            game->textures[i].path = NULL;
        }
        i++;
    }
    
    // Free map grid
    if (game->map.grid)
        free_array(game->map.grid);
}

// Error handling
void error_exit(char *message)
{
    printf("%s", message);
    exit(1);
}
