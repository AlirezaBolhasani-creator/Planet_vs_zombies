#ifndef MOWER_H
#define MOWER_H

#include <stdbool.h>
#include "raylib.h"
#include "config.h"

// forward declaration (بدون include کردن zombie.h)
typedef struct Zombie Zombie;

typedef struct Mower
{
    bool  available;
    bool  active;
    float x;
    float y;
    int row;
    float speed;
} Mower;

void mowers_init(Mower mowers[GRID_ROWS]);
void mowers_update(Mower mowers[GRID_ROWS], Zombie* zombies, int zombieCount, float dt);
void mowers_draw(const Mower mowers[GRID_ROWS], Texture2D tex);
int mower_available_in_row(const Mower mowers[], int row);

#endif
