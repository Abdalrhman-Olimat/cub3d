#include "../include/cub3d.h"

int main(int argc, char **argv)
{
    t_game game;
    
    // Initialize game structure
    ft_memset(&game, 0, sizeof(t_game));
    
    // Phase 2A: Command Line Arguments validation
    if (!parse_arguments(argc, argv))
        return (1);
    
    // Phase 2B: File Reading & Basic Parsing
    if (!parse_file(argv[1], &game))
    {
        cleanup_game(&game);
        return (1);
    }
    
    // Phase 2C: Element Parsing (textures and colors)
    if (!parse_textures_and_colors(&game))
    {
        cleanup_game(&game);
        return (1);
    }
    
    // Phase 2D: Map Parsing
    if (!parse_map(&game))
    {
        cleanup_game(&game);
        return (1);
    }
    
    // Phase 2E: Map Validation
    if (!validate_map(&game))
    {
        cleanup_game(&game);
        return (1);
    }
    
    printf("✓ File parsed successfully!\n");
    printf("✓ Textures and colors validated!\n");
    printf("✓ Map validated!\n");
    printf("✓ Player found at position (%.1f, %.1f) facing %c\n", 
           game.player.x, game.player.y, game.player.orientation);
    
    cleanup_game(&game);
    return (0);
}