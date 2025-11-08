/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_color_parsing.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:10:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:21:05 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static char	*extract_texture_path(char *line, char *identifier)
{
	char	*ptr;
	char	*path_start;
	char	*end_check;

	ptr = line + ft_strlen(identifier);
	ptr = skip_whitespace(ptr);
	if (!*ptr)
		return (NULL);
	path_start = ptr;
	while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
		ptr++;
	end_check = skip_whitespace(ptr);
	if (*end_check && *end_check != '\n')
	{
		printf("Error\nExtra text after texture path\n");
		return (NULL);
	}
	*ptr = '\0';
	return (path_start);
}

int	parse_texture(char *line, t_texture *texture, char *identifier)
{
	char	*path_start;

	path_start = extract_texture_path(line, identifier);
	if (!path_start)
	{
		printf(ERR_TEXTURE);
		return (0);
	}
	if (!validate_texture_file_access(path_start))
		return (0);
	if (!validate_texture_extension(path_start))
		return (0);
	texture->path = ft_strdup(path_start);
	if (!texture->path)
	{
		printf(ERR_MALLOC);
		return (0);
	}
	return (1);
}

int	parse_color(char *line, t_color *color, char identifier)
{
	char	*rgb_start;
	char	**rgb_split;
	int		result;

	rgb_start = get_rgb_start(line);
	if (!rgb_start)
		return (0);
	if (!validate_color_string(rgb_start))
		return (0);
	trim_trailing_whitespace(rgb_start);
	rgb_split = ft_split(rgb_start, ',');
	result = parse_rgb_values(rgb_split, color);
	free_array(rgb_split);
	return (result);
}
