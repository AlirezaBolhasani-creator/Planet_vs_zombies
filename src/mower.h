#ifndef MOWER_H
#define MOWER_H

#include <stdbool.h>
#include "raylib.h"
#include "config.h"
#include "sounds.h"

typedef struct Zombie Zombie;

typedef struct Mower
{
    bool  available; // وجود داشتنش توی ردیف
    bool  active; // راه افتادن توی ردیف
    float x;
    float y;
    int row;
    float speed;
    bool sfx_played;
} Mower;

void mowers_init(Mower mowers[GRID_ROWS]);
void mowers_update(Mower mowers[GRID_ROWS], Zombie* zombies, int zombieCount, float dt);
void mowers_draw(const Mower mowers[GRID_ROWS], Texture2D tex);
#endif
