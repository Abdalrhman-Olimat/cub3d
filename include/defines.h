#ifndef DEFINES_H
# define DEFINES_H

// Map characters
/******************/
# ifdef WALL
#  undef WALL
#  define WALL '1'
# endif
# ifndef WALL
#  define WALL '1'
# endif

# ifdef EMPTY
#  undef EMPTY
#  define EMPTY '0'
# endif
# ifndef EMPTY
#  define EMPTY '0'
# endif

# ifdef NORTH
#  undef NORTH
#  define NORTH 'N'
# endif
# ifndef NORTH
#  define NORTH 'N'
# endif

# ifdef SOUTH
#  undef SOUTH
#  define SOUTH 'S'
# endif
# ifndef SOUTH
#  define SOUTH 'S'
# endif

# ifdef EAST
#  undef EAST
#  define EAST 'E'
# endif
# ifndef EAST
#  define EAST 'E'
# endif

# ifdef WEST
#  undef WEST
#  define WEST 'W'
# endif
# ifndef WEST
#  define WEST 'W'
# endif
/******************/

// Window settings
# ifdef WIN_WIDTH
#  undef WIN_WIDTH
#  define WIN_WIDTH 1024
# endif
# ifndef WIN_WIDTH
#  define WIN_WIDTH 1024
# endif

# ifdef WIN_HEIGHT
#  undef WIN_HEIGHT
#  define WIN_HEIGHT 768
# endif
# ifndef WIN_HEIGHT
#  define WIN_HEIGHT 768
# endif

// Math constants
# ifdef PI
#  undef PI
#  define PI 3.14159265359
# endif
# ifndef PI
#  define PI 3.14159265359
# endif

// Error messages
# ifdef ERR_ARGC
#  undef ERR_ARGC
#  define ERR_ARGC "Error\nWrong number of arguments\n"
# endif
# ifndef ERR_ARGC
#  define ERR_ARGC "Error\nWrong number of arguments\n"
# endif

# ifdef ERR_FILE_EXT
#  undef ERR_FILE_EXT
#  define ERR_FILE_EXT "Error\nFile must have .cub extension\n"
# endif
# ifndef ERR_FILE_EXT
#  define ERR_FILE_EXT "Error\nFile must have .cub extension\n"
# endif

# ifdef ERR_FILE_OPEN
#  undef ERR_FILE_OPEN
#  define ERR_FILE_OPEN "Error\nCannot open file\n"
# endif
# ifndef ERR_FILE_OPEN
#  define ERR_FILE_OPEN "Error\nCannot open file\n"
# endif

# ifdef ERR_FILE_READ
#  undef ERR_FILE_READ
#  define ERR_FILE_READ "Error\nCannot read file\n"
# endif
# ifndef ERR_FILE_READ
#  define ERR_FILE_READ "Error\nCannot read file\n"
# endif

# ifdef ERR_MALLOC
#  undef ERR_MALLOC
#  define ERR_MALLOC "Error\nMemory allocation failed\n"
# endif
# ifndef ERR_MALLOC
#  define ERR_MALLOC "Error\nMemory allocation failed\n"
# endif

# ifdef ERR_TEXTURE
#  undef ERR_TEXTURE
#  define ERR_TEXTURE "Error\nInvalid texture path\n"
# endif
# ifndef ERR_TEXTURE
#  define ERR_TEXTURE "Error\nInvalid texture path\n"
# endif

# ifdef ERR_COLOR
#  undef ERR_COLOR
#  define ERR_COLOR "Error\nInvalid color value\n"
# endif
# ifndef ERR_COLOR
#  define ERR_COLOR "Error\nInvalid color value\n"
# endif

# ifdef ERR_MAP
#  undef ERR_MAP
#  define ERR_MAP "Error\nInvalid map\n"
# endif
# ifndef ERR_MAP
#  define ERR_MAP "Error\nInvalid map\n"
# endif

# ifdef ERR_PLAYER
#  undef ERR_PLAYER
#  define ERR_PLAYER "Error\nInvalid player position\n"
# endif
# ifndef ERR_PLAYER
#  define ERR_PLAYER "Error\nInvalid player position\n"
# endif

#endif