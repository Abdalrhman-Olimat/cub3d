#!/bin/bash

for map in maps/*/*.cub; do
    echo "Running: ./cub3d $map"
    valgrind ./cub3d "$map"
done
