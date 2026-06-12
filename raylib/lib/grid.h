#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include "raylib.h"
#include "config.h"

typedef enum {
    TILE_GRASS_LIGHT = 0,
    TILE_GRASS_DARK  = 1,
    TILE_SOIL        = 2,
    TILE_ASPHALT     = 3,
    TILE_BLOCK       = 4
} TileType;

typedef struct {
    TileType tiles[GRID_ROWS][GRID_COLS]; // [row][col]
} Grid;

void grid_init(Grid* g);

void grid_make_checker_grass(Grid* g);

bool grid_in_bounds(int col, int row);

TileType grid_get(const Grid* g, int col, int row);
void grid_set(Grid* g, int col, int row, TileType t);

bool grid_can_place_plant(const Grid* g, int col, int row);
bool grid_world_to_cell(int x, int y, int* col, int* row);

void grid_draw(const Grid* g, Texture2D grass, Texture2D soil, Texture2D asphalt);

#endif
