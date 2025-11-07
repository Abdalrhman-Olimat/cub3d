/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:45:00 by ahmad             #+#    #+#             */
/*   Updated: 2025/11/07 14:27:18 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	cleanup_game(t_game *game)
{
	int	i;

	if (!game)
		return ;
	if (game->file_content)
		free_array(game->file_content);
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].path)
		{
			free(game->textures[i].path);
			game->textures[i].path = NULL;
		}
		i++;
	}
	if (game->map.grid)
		free_array(game->map.grid);
}

void	error_exit(char *message)
{
	printf("%s", message);
	exit(1);
}
