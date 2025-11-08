/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_line_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 18:10:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/08 18:10:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	handle_texture_line(char *line, t_game *game, int *texture_count)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		if (game->textures[0].path)
			return (printf("Error\nDuplicate texture definition: NO\n"), 0);
		if (!parse_texture(line, &game->textures[0], "NO"))
			return (0);
		(*texture_count)++;
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{
		if (game->textures[1].path)
			return (printf("Error\nDuplicate texture definition: SO\n"), 0);
		if (!parse_texture(line, &game->textures[1], "SO"))
			return (0);
		(*texture_count)++;
	}
	return (1);
}

int	handle_texture_line2(char *line, t_game *game, int *texture_count)
{
	if (ft_strncmp(line, "WE ", 3) == 0)
	{
		if (game->textures[2].path)
			return (printf("Error\nDuplicate texture definition: WE\n"), 0);
		if (!parse_texture(line, &game->textures[2], "WE"))
			return (0);
		(*texture_count)++;
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		if (game->textures[3].path)
			return (printf("Error\nDuplicate texture definition: EA\n"), 0);
		if (!parse_texture(line, &game->textures[3], "EA"))
			return (0);
		(*texture_count)++;
	}
	return (1);
}

int	handle_color_line(char *line, t_game *game, int *floor_set,
		int *ceiling_set)
{
	if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
	{
		if (*floor_set)
			return (printf("Error\nDuplicate floor color definition\n"), 0);
		if (!parse_color(line, &game->floor_color, 'F'))
			return (0);
		*floor_set = 1;
	}
	else if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
	{
		if (*ceiling_set)
			return (printf("Error\nDuplicate ceiling color definition\n"), 0);
		if (!parse_color(line, &game->ceiling_color, 'C'))
			return (0);
		*ceiling_set = 1;
	}
	return (1);
}
