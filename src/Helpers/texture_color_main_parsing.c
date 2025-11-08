/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_color_main_parsing.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:15:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:27:18 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	is_valid_config_line(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0)
		return (1);
	if (ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
		return (1);
	if ((line[0] == 'F' || line[0] == 'C')
		&& (line[1] == ' ' || line[1] == '\t'))
		return (1);
	return (0);
}

static int	process_single_line(char *line, t_game *game, int *counters)
{
	if (!is_valid_config_line(line))
	{
		printf("Error\nInvalid configuration line: %s\n", line);
		return (0);
	}
	if (!handle_texture_line(line, game, &counters[0])
		|| !handle_texture_line2(line, game, &counters[0])
		|| !handle_color_line(line, game, &counters[1], &counters[2]))
		return (0);
	return (1);
}

int	parse_textures_and_colors(t_game *game)
{
	int		i;
	char	*line;
	int		counters[3];

	counters[0] = 0;
	counters[1] = 0;
	counters[2] = 0;
	i = 0;
	while (game->file_content[i])
	{
		line = skip_whitespace(game->file_content[i]);
		if (!*line || line[0] == '1' || line[0] == '0')
		{
			if (line[0] == '1' || line[0] == '0')
				break ;
			i++;
			continue ;
		}
		if (!process_single_line(line, game, counters))
			return (0);
		i++;
	}
	return (validate_textures_and_colors(counters[0], counters[1],
			counters[2]));
}
