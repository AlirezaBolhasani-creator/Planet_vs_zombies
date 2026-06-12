#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "config.h"
#include "plant.h"
#include "sun.h"
#include "mower.h"
#include "bullet.h"
#include "animation.h"
#include "sounds.h"
typedef enum
{
    ZOMBIE_NORMAL,
    ZOMBIE_THINKING
}ZombieType;
typedef enum { ZOMBIE_WALK, ZOMBIE_HIT, ZOMBIE_DIE , ZOMBIE_FLAG_WALK, ZOMBIE_FLAG_HIT} ZombieAnimState;

typedef struct Zombie
{
    bool  active;
    int   health;
    int   row;
    float x;

    float speed;
    float baseSpeed;     // NEW: سرعت اصلی
    float slowTimer;     // NEW: چند ثانیه اسلو مانده

    float attackInterval;
    float attackTimer;
    int   damage;
    ZombieType type;

    Animation anim;
    ZombieAnimState animState;
    bool die_sfx_played;

} Zombie;
void zombies_init(Zombie* zombies, int count);
void zombie_spawn_random_row(Zombie* zombies, int count);
void zombies_update(Zombie* zombies, int count, float dt,Plant plants[GRID_ROWS][GRID_COLS], bool* game_over,
                    float rowMul[GRID_ROWS], const Mower mowers[GRID_ROWS]);
void zombies_draw(const Zombie* zombies, int count);
void zombie_spawn_thinking_random_row(Zombie zombies[], int max);
void zombies_load_animations();
void zombie_set_state(Zombie* z, ZombieAnimState newState);
void zombies_unload_animations();

#endif
