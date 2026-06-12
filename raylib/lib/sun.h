#ifndef MAIN_C_SUN_H
#define MAIN_C_SUN_H

#include "raylib.h"
#include <stdbool.h>
#include "config.h"
typedef struct Sun {
    bool  active;
    bool move;
    int row;
    int col;
    int y;
    float lifeTime;
    float speed;
} Sun;

void draw_sun(int y, int col, Texture2D sun);

void click_on_sun(int* point_sun);

void spawn_sun_random(Sun *suns);
void update_suns(float dt, float* sun_spawn_timer, Sun* suns);
bool handle_sun_click(Vector2 mouse, Sun* suns, int* sun_points);
void draw_suns(Sun* suns, Texture2D sun);
void spawn_sun_at(Sun *suns , int row, int col);

#endif //MAIN_C_SUN_H
