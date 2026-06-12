#ifndef MAIN_C_BULLET_H
#define MAIN_C_BULLET_H
#include <stdbool.h>
#include "config.h"
#include "plant.h"
typedef struct Bullet {
    bool active;
    int row;
    float x;
    float speed;
    int damage;
} Bullet;

void bullet_init(Bullet *bullets);
void spawn_bullet(int row, int col,Bullet *bullets);
void bullet_update(Bullet *bullets, float dt);
void draw_bullets(Bullet *bullets);

#endif //MAIN_C_BULLET_H
