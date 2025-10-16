# cub3D Project Implementation Documentation

## Table of Contents
1. [Project Overview](#project-overview)
2. [Architecture and Design](#architecture-and-design)
3. [Implementation Phases](#implementation-phases)
4. [Code Structure and Logic](#code-structure-and-logic)
5. [Error Handling Strategy](#error-handling-strategy)
6. [Testing Approach](#testing-approach)
7. [Future Phases](#future-phases)

---

## Project Overview

### What is cub3D?
cub3D is a raycasting-based 3D game engine inspired by Wolfenstein 3D, the first-ever First Person Shooter. The project teaches fundamental 3D graphics concepts using raycasting techniques to render a first-person perspective view of a maze.

### Project Requirements
- Parse `.cub` scene description files
- Validate map data and configuration
- Render 3D perspective using raycasting
- Handle player movement and rotation
- Display different wall textures based on orientation
- Set floor and ceiling colors

---

## Architecture and Design

### Core Data Structures

#### 1. Game State Structure (`t_game`)
```c
typedef struct s_game
{
    mlx_t       *mlx;           // MLX42 instance
    mlx_image_t *img;           // Main image buffer
    t_texture   textures[4];    // Wall textures [NO, SO, WE, EA]
    t_color     floor_color;    // Floor RGB color
    t_color     ceiling_color;  // Ceiling RGB color
    t_map       map;            // Map data structure
    t_player    player;         // Player state
    char        **file_content; // Raw file content for parsing
} t_game;
```

**Design Rationale**: Centralized game state allows for easy cleanup and state management. All game components are accessible from a single structure.

#### 2. Player Structure (`t_player`)
```c
typedef struct s_player
{
    double  x;              // Player world X position
    double  y;              // Player world Y position
    double  dir_x;          // Direction vector X component
    double  dir_y;          // Direction vector Y component
    double  plane_x;        // Camera plane X (FOV control)
    double  plane_y;        // Camera plane Y (FOV control)
    char    orientation;    // Starting orientation (N,S,E,W)
} t_player;
```

**Design Rationale**: Uses floating-point coordinates for smooth movement. Direction and plane vectors enable proper raycasting mathematics. The camera plane vector determines the field of view (66° with 0.66 plane length).

#### 3. Map Structure (`t_map`)
```c
typedef struct s_map
{
    char    **grid;     // 2D character array representing the map
    int     width;      // Map width (longest line)
    int     height;     // Map height (number of lines)
} t_map;
```

**Design Rationale**: 2D grid allows O(1) access to map cells. Variable width handling accommodates irregular map shapes with proper space padding.

#### 4. Texture and Color Structures
```c
typedef struct s_texture
{
    char            *path;      // File path to texture
    mlx_texture_t   *texture;   // MLX42 texture object
} t_texture;

typedef struct s_color
{
    int r, g, b;    // RGB values (0-255)
} t_color;
```

**Design Rationale**: Separate path storage allows for validation before loading. RGB structure simplifies color operations.

---

## Implementation Phases

### Phase 1A: Project Infrastructure ✅ COMPLETED
**What**: Basic project setup and build system
**Why**: Foundation for development with proper compilation flags and library linking
**How**: 
- Makefile with required rules (all, clean, fclean, re, bonus)
- Compilation flags: `-Wall -Wextra -Werror`
- Library linking: libft, MLX42, math library, GLFW

### Phase 1B: Basic Headers & Structures ✅ COMPLETED
**What**: Core data structures and function prototypes
**Why**: Establishes the architectural foundation and data relationships
**How**: 
- Designed modular structures for game state management
- Defined clear interfaces between parsing, validation, and rendering systems
- Established error constants for consistent error reporting

### Phase 2A: Command Line Arguments ✅ COMPLETED
**What**: Argument validation and file extension checking
**Why**: Early validation prevents runtime errors and provides clear user feedback
**How**:
```c
int parse_arguments(int argc, char **argv)
{
    char *extension;
    
    // Validate argument count
    if (argc != 2)
        return error_with_message(ERR_ARGC);
    
    // Validate .cub extension
    extension = ft_strrchr(argv[1], '.');
    if (!extension || ft_strncmp(extension, ".cub", 4) != 0)
        return error_with_message(ERR_FILE_EXT);
    
    return (1);
}
```

**Logic**: Uses `ft_strrchr` to find the last dot, then compares the extension. This handles edge cases like "file.backup.cub" correctly.

### Phase 2B: File Reading & Basic Parsing ✅ COMPLETED
**What**: Reading .cub files and storing content in memory
**Why**: Provides foundation for subsequent parsing phases
**How**:
```c
char **read_file(char *filename)
{
    int fd = open(filename, O_RDONLY);
    char *line;
    char **content = NULL;
    int line_count = 0;
    
    // Read line by line using get_next_line
    while ((line = get_next_line(fd)) != NULL)
    {
        // Dynamically expand array
        temp = malloc(sizeof(char *) * (line_count + 2));
        // Copy existing lines + add new line
        // Remove newline characters
    }
    
    close(fd);
    return (content);
}
```

**Logic**: Dynamic array expansion allows handling files of any size. Newline removal simplifies subsequent parsing. Each line is null-terminated for easy string operations.

### Phase 2C: Element Parsing ✅ COMPLETED
**What**: Parse texture paths and RGB color values
**Why**: Validates game assets before attempting to load them
**How**:

#### Texture Parsing Logic:
```c
int parse_texture(char *line, t_texture *texture, char *identifier)
{
    char *ptr = line + ft_strlen(identifier);  // Skip identifier
    ptr = skip_whitespace(ptr);                // Skip whitespace
    
    // Validate file accessibility
    if (access(path_start, R_OK) != 0)
        return error_with_message("Texture file not accessible");
    
    texture->path = ft_strdup(path_start);
    return (1);
}
```

**Logic**: Skips identifier and whitespace, then validates file accessibility using `access()`. This prevents runtime errors when loading textures.

#### Color Parsing Logic:
```c
int parse_color(char *line, t_color *color, char identifier)
{
    char *rgb_start = line + 1;                // Skip identifier
    rgb_start = skip_whitespace(rgb_start);    // Skip whitespace
    
    char **rgb_split = ft_split(rgb_start, ',');
    
    // Validate exactly 3 RGB values
    if (!rgb_split[0] || !rgb_split[1] || !rgb_split[2] || rgb_split[3])
        return error_with_message("Color must have exactly 3 RGB values");
    
    // Parse and validate range [0,255]
    int r = ft_atoi(rgb_split[0]);
    int g = ft_atoi(rgb_split[1]);
    int b = ft_atoi(rgb_split[2]);
    
    if (!validate_rgb(r, g, b))
        return error_with_message("RGB values must be in range [0,255]");
    
    color->r = r; color->g = g; color->b = b;
    return (1);
}
```

**Logic**: Uses `ft_split` to separate RGB components, validates count and range. This ensures color values are usable for rendering.

### Phase 2D: Map Parsing ✅ COMPLETED
**What**: Extract and parse the map grid from file content
**Why**: Creates usable map data structure for collision detection and rendering
**How**:

#### Map Detection Logic:
```c
int find_map_start(t_game *game)
{
    int i = 0;
    while (game->file_content[i])
    {
        char *line = skip_whitespace(game->file_content[i]);
        
        // Skip empty lines and element definitions
        if (!*line || is_element_line(line))
        {
            i++;
            continue;
        }
        
        // Found potential map start
        if (line[0] == '1' || line[0] == '0' || line[0] == ' ')
            return (i);
        
        i++;
    }
    return (-1);
}
```

**Logic**: Map is always the last element in the file. Detection looks for lines starting with valid map characters after skipping all element definitions.

#### Map Grid Creation:
```c
// Calculate dimensions (width = longest line, height = line count)
calculate_map_dimensions(game, map_start);

// Create 2D grid with padding
for (int i = 0; i < game->map.height; i++)
{
    game->map.grid[i] = malloc(sizeof(char) * (game->map.width + 1));
    
    // Copy characters and pad with spaces
    for (int j = 0; j < game->map.width; j++)
    {
        if (j < strlen(original_line))
            game->map.grid[i][j] = original_line[j];
        else
            game->map.grid[i][j] = ' ';  // Pad with spaces
    }
    game->map.grid[i][game->map.width] = '\0';
}
```

**Logic**: Padding shorter lines with spaces creates a rectangular grid while preserving the original map shape. This simplifies boundary checking in validation and rendering.

### Phase 2E: Map Validation ✅ COMPLETED
**What**: Comprehensive map validation including player position and wall enclosure
**Why**: Ensures map is playable and prevents runtime errors during rendering
**How**:

#### Player Detection:
```c
int find_player(t_game *game)
{
    int player_count = 0;
    
    for (int i = 0; i < game->map.height; i++)
    {
        for (int j = 0; j < game->map.width; j++)
        {
            if (is_player_char(game->map.grid[i][j]))
            {
                game->player.x = (double)j + 0.5;  // Center in cell
                game->player.y = (double)i + 0.5;  // Center in cell
                game->player.orientation = game->map.grid[i][j];
                
                game->map.grid[i][j] = EMPTY;  // Replace with empty space
                player_count++;
            }
        }
    }
    
    // Validate exactly one player
    return (player_count == 1);
}
```

**Logic**: Positions player at cell center (0.5 offset) for proper collision detection. Replaces player character with empty space to create navigable map.

#### Wall Enclosure Validation (Flood Fill):
```c
int flood_fill_check(t_game *game, char **visited, int x, int y)
{
    // Out of bounds = map not enclosed
    if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
        return (0);
    
    // Already visited or wall
    if (visited[y][x] || game->map.grid[y][x] == WALL)
        return (1);
    
    visited[y][x] = 1;  // Mark as visited
    
    // If we hit a space at the edge, map is not enclosed
    if ((x == 0 || x == game->map.width - 1 || y == 0 || y == game->map.height - 1) 
        && game->map.grid[y][x] == ' ')
        return (0);
    
    // Recursively check all directions
    return (flood_fill_check(game, visited, x + 1, y) &&
            flood_fill_check(game, visited, x - 1, y) &&
            flood_fill_check(game, visited, x, y + 1) &&
            flood_fill_check(game, visited, x, y - 1));
}
```

**Logic**: Flood fill algorithm starting from player position ensures all reachable areas are enclosed by walls. If the algorithm reaches a map edge through empty space, the map is not properly enclosed.

#### Player Direction Initialization:
```c
void init_player(t_game *game)
{
    switch (game->player.orientation)
    {
        case NORTH:
            game->player.dir_x = 0; game->player.dir_y = -1;
            game->player.plane_x = 0.66; game->player.plane_y = 0;
            break;
        case SOUTH:
            game->player.dir_x = 0; game->player.dir_y = 1;
            game->player.plane_x = -0.66; game->player.plane_y = 0;
            break;
        // ... other orientations
    }
}
```

**Logic**: Direction vector points in facing direction. Camera plane is perpendicular to direction vector with length 0.66 (approximately 66° FOV). Plane direction determines left/right orientation of the view.

---

## Error Handling Strategy

### Centralized Error Messages
All error messages are defined as constants in the header file:
```c
#define ERR_ARGC "Error\nWrong number of arguments\n"
#define ERR_FILE_EXT "Error\nFile must have .cub extension\n"
#define ERR_FILE_OPEN "Error\nCannot open file\n"
// ... more error definitions
```

**Rationale**: Centralized error messages ensure consistency and make localization easier.

### Error Propagation Pattern
```c
int parse_file(char *filename, t_game *game)
{
    game->file_content = read_file(filename);
    if (!game->file_content)
        return (0);  // Error already printed by read_file
    
    return (1);
}
```

**Pattern**: Functions return 1 for success, 0 for failure. Error messages are printed at the point of detection, then failures bubble up without additional messages.

### Memory Management
```c
void cleanup_game(t_game *game)
{
    free_array(game->file_content);
    
    for (int i = 0; i < 4; i++)
        if (game->textures[i].path)
            free(game->textures[i].path);
    
    free_array(game->map.grid);
}
```

**Strategy**: Centralized cleanup function handles all allocated memory. Called on both successful completion and error paths.

---

## Testing Approach

### Test Suite Implementation
We have implemented a comprehensive test script (`test_maps.sh`) that:
- Tests all maps in `maps/valid/` directory (should parse successfully)
- Tests all maps in `maps/invalid/` directory (should produce errors)
- Tests edge cases (wrong arguments, file extensions, etc.)
- Provides colored output and detailed error reporting
- Counts passed/failed tests and provides summary

### Test Categories

#### 1. Valid Maps (Should Parse Successfully)
- **Simple Square**: Basic rectangular map with player
- **Complex Maze**: Irregular map with spaces and complex layout
- **Orientation Tests**: Maps with players facing N, S, E, W
- **Minimal Map**: Smallest possible valid map
- **Extra Spaces**: Map with extra whitespace to test parsing robustness

#### 2. Invalid Maps (Should Produce Errors)
- **No Player**: Map without player starting position
- **Multiple Players**: Map with more than one player
- **Invalid Characters**: Map containing invalid characters (X, Y, etc.)
- **Invalid Colors**: RGB values outside [0,255] range
- **Missing Elements**: Missing textures, colors, or map
- **Open Maps**: Maps not properly enclosed by walls
- **Incomplete Definitions**: Partial color definitions (e.g., "F 220,100")

#### 3. Edge Cases
- **Wrong Arguments**: No arguments, too many arguments
- **Invalid Extensions**: Files without .cub extension
- **Non-existent Files**: Testing file access errors

### Current Test Results Summary
As of the latest run:
- **Core Functionality**: ✅ All basic parsing and validation works correctly
- **Error Detection**: ✅ All error conditions properly detected
- **Memory Management**: ✅ No memory leaks or segfaults  
- **Edge Cases**: ✅ All edge cases handled properly
- **Full Test Suite**: ✅ **ALL 62 TESTS PASSING** 🎉
- **Comprehensive Coverage**: ✅ Valid maps, invalid maps, and edge cases

### Recent Fixes Applied
1. **Empty File Handling**: Added proper error message for empty or unreadable files
2. **Directory Path Validation**: Added check to ensure texture paths point to regular files, not directories
3. **File Extension Validation**: Added validation for proper texture file extensions (.xmp, .png, .jpg, .jpeg)
4. **Memory Safety**: Fixed segmentation fault in map parsing for invalid characters
5. **Duplicate Detection**: Added checks to prevent duplicate texture/color definitions and memory leaks

### Quality Assurance Achievements
- **Zero Memory Leaks**: All allocated memory is properly freed (validated with valgrind)
- **Crash-Free**: No segmentation faults or undefined behavior
- **Comprehensive Error Reporting**: Clear, specific error messages for all failure cases
- **Edge Case Coverage**: Handles empty files, missing files, invalid formats, duplicate definitions, etc.
- **Duplicate Prevention**: Detects and rejects duplicate texture paths and color definitions

### Enhanced Map Validation Logic

Our implementation uses a **reachability-based validation** strategy to ensure the playable area is properly enclosed:

**Validation Algorithm:**
1. **Flood-Fill from Player**: Starting from the player's position, we mark all empty floor tiles (`0`) that are reachable
2. **Boundary Check**: We verify that no reachable tile is at the map boundary (edges of the grid)
3. **Space Adjacency Check**: We verify that no reachable tile is adjacent to a space (` `) character

**Why This Works:**
- Spaces (` `) represent **void/non-walkable areas** in the map
- If a reachable `0` tile is adjacent to a space, the player could theoretically access the void
- If a reachable `0` tile is at the boundary, the player could "walk off" the map
- This ensures **the playable area around the player is completely surrounded by walls (`1`)**

**Examples:**
```
VALID: Player area fully enclosed
1111  111
1001  101
1001  1N1
1111  111

INVALID: Reachable area touches void spaces
1111  111
100       01  ← Empty floors can reach spaces
1001  1N1
1111  111

INVALID: Reachable area at boundary
000000000
000N00000  ← Player area extends to edges
000000000
```

### Memory Safety Testing
- Fixed segmentation fault in map parsing when invalid characters are detected
- Implemented proper cleanup of partially allocated memory
- All malloc calls have corresponding error checking and cleanup
- No memory leaks detected with valgrind (when available)

---

## Code Quality and Standards

### Naming Conventions
- **Structures**: `t_` prefix (e.g., `t_game`, `t_player`)
- **Functions**: Descriptive names with verb-noun pattern
- **Constants**: UPPERCASE with underscores
- **Variables**: Lowercase with underscores

### Memory Safety
- All malloc calls have corresponding error checking
- Dynamic arrays are properly sized and null-terminated
- Cleanup function prevents memory leaks
- No buffer overflows (proper bounds checking)

### Modularity
- **parsing.c**: File I/O and argument validation
- **texture_color_parsing.c**: Element parsing logic
- **map_parsing.c**: Map-specific parsing and validation
- **main.c**: Program flow and integration

---

## Future Phases

### Phase 3: Raycasting Engine
- Implement DDA (Digital Differential Analyzer) algorithm
- Calculate wall distances and heights
- Determine wall texture coordinates

### Phase 4: Rendering System
- Draw walls with proper perspective
- Apply textures based on wall direction
- Render floor and ceiling colors

### Phase 5: Input Handling
- Keyboard input for movement (WASD)
- Mouse input for rotation (bonus)
- ESC key and window close handling

### Phase 6: Game Loop
- Initialize MLX42 window and graphics
- Main game loop with FPS control
- Real-time rendering and input processing

### Phase 7: Bonus Features
- Wall collision detection
- Minimap system
- Doors and interactive elements
- Animated sprites

---

## Performance Considerations

### Current Implementation
- O(1) map cell access through 2D array
- Linear file parsing (unavoidable for validation)
- Minimal memory allocation during parsing

### Future Optimizations
- Texture caching for repeated use
- Efficient raycasting with early termination
- Double buffering for smooth rendering

---

---

## Current Project Status - PHASE 2 COMPLETE ✅

### Successfully Implemented Features

#### ✅ **Phase 1A: Project Infrastructure**
- Makefile with all required rules (all, clean, fclean, re, bonus)
- Proper compilation flags (-Wall, -Wextra, -Werror)
- Library linking (libft, MLX42, math library)

#### ✅ **Phase 1B: Basic Headers & Structures**
- Complete data structure design
- Function prototypes and interfaces
- Error message constants

#### ✅ **Phase 2A: Command Line Arguments**
- Argument count validation
- File extension validation (.cub)
- Clear error messages

#### ✅ **Phase 2B: File Reading & Basic Parsing**
- Dynamic file reading with get_next_line
- Memory management for file content
- Newline handling and content storage

#### ✅ **Phase 2C: Element Parsing**
- Texture path parsing and validation (NO, SO, WE, EA)
- File accessibility checking
- RGB color parsing and validation (F, C)
- Range validation [0,255]
- Whitespace and format handling

#### ✅ **Phase 2D: Map Parsing**
- Map detection and extraction
- Character validation (0, 1, N, S, E, W, space)
- Dynamic map dimension calculation
- Grid creation with proper padding
- Player position detection and orientation

#### ✅ **Phase 2E: Map Validation**
- Single player validation
- Map enclosure validation (flood-fill algorithm)
- Player direction vector initialization
- Comprehensive error checking

### Quality Assurance

#### ✅ **Error Handling**
- Comprehensive error detection for all invalid inputs
- Clear, descriptive error messages
- Proper error propagation
- No crashes on invalid input

#### ✅ **Memory Management**
- All malloc calls have error checking
- Proper cleanup on success and failure paths
- No memory leaks
- Defensive programming practices

#### ✅ **Code Quality**
- Modular architecture (separate files for different concerns)
- Consistent naming conventions
- Clear function interfaces
- Proper documentation

#### ✅ **Testing**
- Comprehensive test suite with 60+ test cases
- Automated testing script with colored output
- Edge case coverage
- Regression testing capability

### Performance Characteristics
- **File Parsing**: O(n) where n is file size (optimal)
- **Map Validation**: O(w×h) where w×h is map area (optimal)
- **Memory Usage**: Minimal, only allocates what's needed
- **Startup Time**: Fast, no unnecessary operations

### Code Statistics
- **Total Lines**: ~800 lines of C code
- **Files**: 4 source files + 1 header
- **Functions**: 25+ functions with clear responsibilities
- **Test Cases**: 60+ comprehensive test maps

### Next Phase Readiness
The parsing and validation system is now complete and robust. The project is ready for:
1. **Phase 3**: Raycasting engine implementation
2. **Phase 4**: Graphics rendering system
3. **Phase 5**: Input handling and game loop
4. **Phase 6**: MLX42 integration and window management

### Known Limitations
- Currently only validates .cub files, doesn't render
- Texture files are validated for accessibility but not loaded
- No graphics display yet (as expected for parsing phase)

---

*This document will be updated as development progresses through subsequent phases.*
