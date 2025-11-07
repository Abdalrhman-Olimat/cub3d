/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reading_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:17:32 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	**add_line_to_content(char **content, char *line, int line_count)
{
	char	**temp;
	int		i;

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

int	open_file_fd(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf(ERR_FILE_OPEN);
		return (-1);
	}
	return (fd);
}

int	process_file_lines(int fd, char ***content)
{
	char	*line;
	int		line_count;

	*content = NULL;
	line_count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		*content = add_line_to_content(*content, line, line_count);
		if (!*content)
		{
			close(fd);
			return (-1);
		}
		line_count++;
		line = get_next_line(fd);
	}
	return (line_count);
}

int	validate_file_content(int line_count)
{
	if (line_count == 0)
	{
		printf("Error\nEmpty file or file reading failed\n");
		return (0);
	}
	return (1);
}
