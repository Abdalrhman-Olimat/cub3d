/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 18:00:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/08 18:00:00 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	is_valid_color_char(char c)
{
	if (c == ' ' || c == '\t' || c == ',' || c == '\n')
		return (1);
	if (ft_isdigit(c))
		return (1);
	return (0);
}

int	validate_color_string(char *rgb_start)
{
	int	i;

	i = 0;
	while (rgb_start[i])
	{
		if (!is_valid_color_char(rgb_start[i]))
		{
			printf("Error\nInvalid character in color values\n");
			return (0);
		}
		i++;
	}
	return (1);
}

char	*get_rgb_start(char *line)
{
	char	*ptr;

	ptr = line + 1;
	ptr = skip_whitespace(ptr);
	if (!*ptr)
	{
		printf(ERR_COLOR);
		return (NULL);
	}
	return (ptr);
}

int	validate_rgb_array(char **rgb_split)
{
	if (!rgb_split)
	{
		printf("Error\nColor must have exactly 3 RGB values\n");
		return (0);
	}
	if (!rgb_split[0] || !rgb_split[1] || !rgb_split[2] || rgb_split[3])
	{
		printf("Error\nColor must have exactly 3 RGB values\n");
		return (0);
	}
	return (1);
}

int	set_color_values(t_color *color, int r, int g, int b)
{
	color->r = r;
	color->g = g;
	color->b = b;
	return (1);
}
