#ifndef PLANT_H
#define PLANT_H
#include <stdbool.h>
#include "grid.h"
#include "animation.h"
typedef enum
{
    PLANT_NONE,
    PLANT_PEA,
    PLANT_SUNFLOWER,
    PLANT_WALLNUT,
    PLANT_CHOMPER
}plant_type;
// هزینه‌ی هر گیاه برحسب امتیاز خورشید
static const int PLANT_COST[] =
        {
        0,   // PLANT_NONE
        100, // PLANT_PEA
        50,  // PLANT_SUNFLOWER
        75,   // PLANT_WALLNUT
        150 // PLANT_CHOMPER
};
typedef enum { EVENT_NONE, EVENT_SPAWN_BULLET, EVENT_SPAWN_SUN } PlantEventType;

typedef struct {
    PlantEventType type;
    int row, col;
} PlantEvent;

typedef struct
{
    plant_type type;
    bool alive;
    int health;
    int maxHealth;

    float cooldown;       // مدت کولداون (ثانیه)
    float cooldownTimer;  // تایمر باقی‌مانده (ثانیه)

    float actionInterval; // فاصله‌ی عمل (شلیک/تولید خورشید)
    float actionTimer;
    int col;
    int row;

    //just for chomper
    float lifeTime;       // کل عمر
    float lifeTimer;      // تایمر باقی‌مانده
    Animation anim;
}Plant;
typedef struct
{
    Texture2D pea;
    Texture2D sunflower;
    Texture2D wallnut;
    Texture2D chomper_tex;

} PlantTextures;
void plants_load_anim();

void plant_set_textures(PlantTextures tex);
static inline bool plant_is_valid(plant_type t)
{
    return t != PLANT_NONE;
}
void plant_grid_clear(Plant plants[GRID_ROWS][GRID_COLS]);
void plant_grid_draw(const Plant plants[GRID_ROWS][GRID_COLS]);
bool plant_cell_is_empty(const Plant plants[GRID_ROWS][GRID_COLS], int col, int row);
bool plant_place(Plant plants[GRID_ROWS][GRID_COLS], plant_type t, int col, int row);
bool plant_remove(Plant plants[GRID_ROWS][GRID_COLS], int col, int row);
void plant_draw_preview(Plant plants[GRID_ROWS][GRID_COLS],plant_type t, int col, int row, Color tint);
void plant_init_cel(Plant *p, plant_type t);
void plant_update_all(Plant plants[GRID_ROWS][GRID_COLS], float dt);
int plant_collect_events(Plant plants[GRID_ROWS][GRID_COLS], float dt,PlantEvent* out_events, int max_events);
int count_plants_in_row(const Plant plants[GRID_ROWS][GRID_COLS], int row);
void apply_rose_effects(Plant plants[GRID_ROWS][GRID_COLS], float dt, float rose_heal_acc[GRID_ROWS]);


#endif
