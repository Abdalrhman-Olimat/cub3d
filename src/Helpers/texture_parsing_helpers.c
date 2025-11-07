/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parsing_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:00:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:27:18 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	*skip_whitespace(char *str)
{
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	return (str);
}

void	trim_trailing_whitespace(char *str)
{
	int	len;

	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t'))
	{
		str[len - 1] = '\0';
		len--;
	}
}

int	validate_texture_file_access(char *path_start)
{
	struct stat	file_stat;

	if (access(path_start, R_OK) != 0)
	{
		printf("Error\nTexture file not accessible: %s\n", path_start);
		return (0);
	}
	if (stat(path_start, &file_stat) != 0 || !S_ISREG(file_stat.st_mode))
	{
		printf("Error\nTexture path is not a regular file: %s\n", path_start);
		return (0);
	}
	return (1);
}

int	validate_texture_extension(char *path_start)
{
	char	*extension;

	extension = ft_strrchr(path_start, '.');
	if (!extension)
	{
		printf("Error\nTexture file must have valid extension\n");
		return (0);
	}
	if (ft_strncmp(extension, ".xpm", 4) != 0
		&& ft_strncmp(extension, ".png", 4) != 0
		&& ft_strncmp(extension, ".jpg", 4) != 0
		&& ft_strncmp(extension, ".jpeg", 5) != 0)
	{
		printf("Error\nTexture file must have valid extension\n");
		return (0);
	}
	return (1);
}
