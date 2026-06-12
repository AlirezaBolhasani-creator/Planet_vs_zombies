#ifndef PVZ_LEVELS_H
#define PVZ_LEVELS_H
#include "raylib.h"
typedef enum
{
    level_one,
    level_two,
    level_three,
    level_four,
    level_none
}level_res;
level_res update_level();
void level_show(Texture2D level_picture);
#endif //PVZ_LEVELS_H
