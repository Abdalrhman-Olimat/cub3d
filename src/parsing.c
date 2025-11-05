/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeleimat <aeleimat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:37:08 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/05 16:32:48 by aeleimat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

int parse_file(char *filename, t_game *game)
{
    game->file_content = read_file(filename);
    if (!game->file_content)
        return (0);
    
    return (1);
}

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

void cleanup_game(t_game *game)
{
    int i;
    
    if (!game)
        return;
    if (game->file_content)
        free_array(game->file_content);
    
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
    if (game->map.grid)
        free_array(game->map.grid);
}

void error_exit(char *message)
{
    printf("%s", message);
    exit(1);
}
