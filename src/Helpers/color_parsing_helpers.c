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

	if (!validate_rgb_array(rgb_split))
		return (0);
	r = ft_atoi(rgb_split[0]);
	g = ft_atoi(rgb_split[1]);
	b = ft_atoi(rgb_split[2]);
	if (!validate_rgb(r, g, b))
	{
		printf("Error\nRGB values must be in range [0,255]\n");
		return (0);
	}
	return (set_color_values(color, r, g, b));
}

static int	check_texture_count(int texture_count)
{
	if (texture_count != 4)
	{
		printf("Error\nMissing texture definitions (need NO, SO, WE, EA)\n");
		return (0);
	}
	return (1);
}

static int	check_floor_ceiling(int floor_set, int ceiling_set)
{
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

int	validate_textures_and_colors(int texture_count, int floor_set,
		int ceiling_set)
{
	if (!check_texture_count(texture_count))
		return (0);
	if (!check_floor_ceiling(floor_set, ceiling_set))
		return (0);
	return (1);
}
