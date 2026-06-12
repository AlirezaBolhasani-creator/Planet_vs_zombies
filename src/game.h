#ifndef PVZ_GAME_H
#define PVZ_GAME_H
#include <stdbool.h>


enum game_state {
    STATE_MENU,
    STATE_LEVEL_SELECT,
    STATE_LEVEL1,
    STATE_LEVEL2,
    STATE_LEVEL3,
    STATE_LEVEL4,
    STATE_SHOP,
    STATE_EXIT,
    STATE_GAME_OVER,
    STATE_LEVEL_COMPLETE
};


void game_update(enum game_state* game, float dt);
void game_draw(enum  game_state* game);
void game_init();
void game_reset();
void game_unload();

#endif //PVZ_GAME_H
