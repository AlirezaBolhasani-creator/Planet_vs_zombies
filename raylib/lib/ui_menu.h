#ifndef PVZ_UI_MENU_H
#define PVZ_UI_MENU_H
#include "raylib.h"
typedef enum {
    menu_play,
    menu_exit,
    menu_shop,
    menu_none
}menu_res;
menu_res menu_update();
void menu_show(Texture2D background);
#endif //PVZ_UI_MENU_H
