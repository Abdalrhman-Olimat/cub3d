/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_parsing_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:05:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:27:18 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	validate_rgb(int r, int g, int b)
{
	return (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255);
}

int	parse_rgb_values(char **rgb_split, t_color *color)
{
	int	r;
	int	g;
	int	b;

	if (!rgb_split || !rgb_split[0] || !rgb_split[1] || !rgb_split[2]
		|| rgb_split[3])
	{
		free_array(rgb_split);
		printf("Error\nColor must have exactly 3 RGB values\n");
		return (0);
	}
	r = ft_atoi(rgb_split[0]);
	g = ft_atoi(rgb_split[1]);
	b = ft_atoi(rgb_split[2]);
	if (!validate_rgb(r, g, b))
	{
		printf("Error\nRGB values must be in range [0,255]\n");
		return (0);
	}
	color->r = r;
	color->g = g;
	color->b = b;
	return (1);
}

int	validate_textures_and_colors(int texture_count, int floor_set,
		int ceiling_set)
{
	if (texture_count != 4)
	{
		printf("Error\nMissing texture definitions (need NO, SO, WE, EA)\n");
		return (0);
	}
	if (!floor_set)
	{
		printf("Error\nMissing floor color definition (F)\n");
		return (0);
	}
	if (!ceiling_set)
	{
		printf("Error\nMissing ceiling color definition (C)\n");
		return (0);
	}
	return (1);
}
