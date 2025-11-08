/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:37:08 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/08 09:43:34 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	parse_arguments(int argc, char **argv)
{
	char	*extension;
	int		len;

	if (argc != 2)
	{
		printf(ERR_ARGC);
		return (0);
	}
	extension = ft_strrchr(argv[1], '.');
	if (!extension)
	{
		printf(ERR_FILE_EXT);
		return (0);
	}
	len = ft_strlen(extension);
	if (len != 4 || ft_strncmp(extension, ".cub", 4) != 0)
	{
		printf(ERR_FILE_EXT);
		return (0);
	}
	return (1);
}

char	**read_file(char *filename)
{
	int		fd;
	int		line_count;
	char	**content;

	fd = open_file_fd(filename);
	if (fd < 0)
		return (NULL);
	line_count = process_file_lines(fd, &content);
	close(fd);
	if (line_count < 0)
		return (NULL);
	if (!validate_file_content(line_count))
		return (NULL);
	return (content);
}

int	parse_file(char *filename, t_game *game)
{
	game->file_content = read_file(filename);
	if (!game->file_content)
		return (0);
	return (1);
}
