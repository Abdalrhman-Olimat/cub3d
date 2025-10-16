# Memory Leak Fix - Duplicate Definitions

## Issue Discovered
Date: October 16, 2025

When running valgrind on a .cub file with duplicate texture definitions:
```bash
valgrind --leak-check=full ./cub3d maps/invalid/textures_duplicates.cub
```

**Result**: 23 bytes memory leak detected

## Root Cause Analysis

### The Problem
When parsing texture/color definitions, the parser would:
1. Parse first occurrence (e.g., `EA textures/test/east.png`) → allocate memory via `ft_strdup`
2. Parse duplicate occurrence (e.g., second `EA` line) → allocate NEW memory
3. Overwrite the pointer without freeing the first allocation
4. Return error due to `texture_count != 4` (it would be 5)
5. **First allocation was now leaked** (unreachable memory)

### Example Scenario
```
NO textures/test/north.png  ← Allocated, stored
SO textures/test/south.png  ← Allocated, stored
WE textures/test/west.png   ← Allocated, stored
EA textures/test/east.png   ← Allocated, stored in game->textures[3].path
EA textures/test/east.png   ← Allocated AGAIN, overwrites game->textures[3].path
                               (first allocation now leaked!)
```

## Solution Implemented

### Code Changes
Added duplicate detection **before** memory allocation in `src/texture_color_parsing.c`:

```c
// Parse North texture
if (ft_strncmp(line, "NO ", 3) == 0)
{
    if (game->textures[0].path)  // ← Check if already set!
    {
        printf("Error\nDuplicate texture definition: NO\n");
        return (0);  // Exit before allocating new memory
    }
    if (!parse_texture(line, &game->textures[0], "NO"))
        return (0);
    texture_count++;
}
```

Applied to all texture definitions:
- `NO` (North)
- `SO` (South)
- `WE` (West)
- `EA` (East)

And color definitions:
- `F` (Floor color)
- `C` (Ceiling color)

### Benefits
1. **Prevents Memory Leaks**: No allocation happens if duplicate detected
2. **Better Error Messages**: User gets specific "Duplicate X definition" error
3. **Early Detection**: Fails fast before unnecessary processing
4. **Consistent Behavior**: Same validation for textures and colors

## Validation Results

### Before Fix
```
==38839== HEAP SUMMARY:
==38839==     in use at exit: 23 bytes in 1 blocks
==38839==   total heap usage: 107 allocs, 106 frees, 29,154 bytes allocated
==38839== 
==38839== 23 bytes in 1 blocks are definitely lost
==38839== LEAK SUMMARY:
==38839==    definitely lost: 23 bytes in 1 blocks
```

### After Fix
```
==39433== HEAP SUMMARY:
==39433==     in use at exit: 0 bytes in 0 blocks
==39433==   total heap usage: 98 allocs, 98 frees, 29,046 bytes allocated
==39433== 
==39433== All heap blocks were freed -- no leaks are possible
==39433== 
==39433== ERROR SUMMARY: 0 errors from 0 contexts
```

✅ **No memory leaks!**

## Test Cases Verified

### 1. Duplicate Texture
```bash
valgrind --leak-check=full ./cub3d maps/invalid/textures_duplicates.cub
# Result: No leaks, proper error message
```

### 2. Duplicate Color
```bash
valgrind --leak-check=full ./cub3d maps/invalid/color_duplicates.cub
# Result: No leaks, proper error message
```

### 3. Valid Map
```bash
valgrind --leak-check=full ./cub3d maps/valid/simple_square.cub
# Result: No leaks, successful parsing
```

### 4. Other Invalid Cases
```bash
valgrind --leak-check=full ./cub3d maps/invalid/missing_texture.cub
# Result: No leaks, proper error handling
```

### 5. Full Test Suite
```bash
./test_maps.sh
# Result: 62/62 tests passing
```

## Impact

### Memory Safety Improvements
- ✅ Zero memory leaks in all scenarios
- ✅ Proper error handling for duplicate definitions
- ✅ No performance regression
- ✅ Clearer error messages for users

### Code Quality
- Added 6 duplicate checks (4 textures + 2 colors)
- Improved early validation strategy
- Consistent error handling pattern
- Better user experience with specific error messages

## Conclusion

The memory leak has been successfully fixed by implementing **pre-allocation validation**. The parser now checks for duplicates before allocating memory, preventing leaks and providing better error feedback.

**Status**: ✅ RESOLVED - All valgrind tests passing with zero leaks
