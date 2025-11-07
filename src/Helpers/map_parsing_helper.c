/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:40:01 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/07 14:59:02 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
/*
 * map_parsing_helper.c functions are now provided by src/map_parsing.c
 * (working implementation). Disable these definitions here to avoid
 * duplicate symbol errors at link time. Kept for reference.
 */

#if 0
int	is_valid_map_char(char c)
{
	return (c == WALL || c == EMPTY || c == NORTH || c == SOUTH
		|| c == EAST || c == WEST || c == ' ');
}

int	is_player_char(char c)
{
	return (c == NORTH || c == SOUTH || c == EAST || c == WEST);
}

int	is_texture_or_color_line(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0
		|| ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0
		|| ft_strncmp(line, "EA ", 3) == 0
		|| line[0] == 'F' || line[0] == 'C')
		return (1);
	return (0);
}

int	find_map_start(t_game *game)
{
	int		i;
	char	*line;

	i = 0;
	while (game->file_content[i])
	{
		line = skip_whitespace(game->file_content[i]);
		if (!*line)
		{
			i++;
			continue ;
		}
		if (is_texture_or_color_line(line))
		{
			i++;
			continue ;
		}
		if (line[0] == '1' || line[0] == '0' || line[0] == ' ')
			return (i);
		i++;
	}
	return (-1);
}

void	calculate_map_dimensions(t_game *game, int map_start)
{
	int	i;
	int	max_width;
	int	current_width;

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
#endif
