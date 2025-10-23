#include "../include/cub3d.h"

/*
P2A:
Parse command line arguments to
make sure there is exactly one argument (the .cub file)
check that the file has a .cub extension
*/
int parse_arguments(int argc, char **argv)
{
    char *extension;
    
    if (argc != 2)
    {
        printf(ERR_ARGC);
        return (0);
    }
    extension = ft_strrchr(argv[1], '.');
    if (!extension || ft_strncmp(extension, ".cub", 4) != 0)
    {
        printf(ERR_FILE_EXT);
        return (0);
    }
    return (1);
}

// Phase 2B: File Reading & Basic Parsing
/*
Helper function to add a line to the content array
*/
static char **add_line_to_content(char **content, char *line, int line_count)
{
    char **temp;
    int i;
    
    temp = malloc(sizeof(char *) * (line_count + 2));
    if (!temp)
    {
        free_array(content);
        free(line);
        printf(ERR_MALLOC);
        return (NULL);
    }
    i = 0;
    while (i < line_count && content)
    {
        temp[i] = content[i];
        i++;
    }
    if (line[ft_strlen(line) - 1] == '\n')
        line[ft_strlen(line) - 1] = '\0';
    temp[line_count] = line;
    temp[line_count + 1] = NULL;
    free(content);
    return (temp);
}

/*
Read file and store content in array
*/
char **read_file(char *filename)
{
    int fd;
    char *line;
    char **content;
    int line_count;
    
    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        printf(ERR_FILE_OPEN);
        return (NULL);
    }
    content = NULL;
    line_count = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        content = add_line_to_content(content, line, line_count);
        if (!content)
        {
            close(fd);
            return (NULL);
        }
        line_count++;
    }
    close(fd);
    if (line_count == 0)
    {
        printf("Error\nEmpty file or file reading failed\n");
        return (NULL);
    }
    return (content);
}

/*
Phase 2B: File Reading & Basic Parsing
- Read the .cub file line by line
- Store each line in a dynamically allocated array of strings in the game structure
*/
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
