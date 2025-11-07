/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmad <ahmad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:40:01 by aeleimat          #+#    #+#             */
/*   Updated: 2025/11/07 10:38:43 by ahmad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	print_initing_res(void)
{
	printf("✓ Graphics initialized!\n");
	printf("✓ Window created: %dx%d\n", WIN_WIDTH, WIN_HEIGHT);
	printf("✓ Textures loaded successfully!\n");
	printf("\n🚀 Starting game...\n");
	printf("Controls:\n");
	printf("  W/S     - Move forward/backward\n");
	printf("  A/D     - Strafe left/right\n");
	printf("  ←/→     - Rotate camera\n");
	printf("  ESC     - Exit\n");
}

int	parse_game_data(char *filename, t_game *game)
{
	if (!parse_file(filename, game))
		return (0);
	if (!parse_textures_and_colors(game))
		return (0);
	if (!parse_map(game))
		return (0);
	if (!validate_map(game))
		return (0);
	return (1);
}

void	print_validation_results(t_game *game)
{
	printf("✓ File parsed successfully!\n");
	printf("✓ Textures and colors validated!\n");
	printf("✓ Map validated!\n");
	printf("✓ Player found at position (%.1f, %.1f) facing %c\n",
		game->player.x, game->player.y, game->player.orientation);
}

int	initialize_and_start_game(t_game *game)
{
	printf("\n🎮 Initializing graphics...\n");
	if (!init_graphics(game))
		return (0);
	print_initing_res();
	start_game_loop(game);
	return (1);
}

int	main(int argc, char **argv)
{
	t_game	game;

	ft_memset(&game, 0, sizeof(t_game));
	if (!parse_arguments(argc, argv))
		return (1);
	if (!parse_game_data(argv[1], &game))
	{
		cleanup_game(&game);
		return (1);
	}
	print_validation_results(&game);
	if (!initialize_and_start_game(&game))
	{
		cleanup_game(&game);
		return (1);
	}
	cleanup_graphics(&game);
	cleanup_game(&game);
	return (0);
}
