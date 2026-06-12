#ifndef MAIN_C_SOUNDS_H
#define MAIN_C_SOUNDS_H
#include "raylib.h"
#include "config.h"
typedef enum {
    SFX_PLANT,
    SFX_THROW,
    SFX_SPLAT,
    SFX_POINTS,
    SFX_ZOMBIE_DIE,
    SFX_MOWER,
    SFX_CLICK,
    SFX_ERROR
} SfxId;
void play_sfx(SfxId id);
void init_sounds();
void unload_sounds();
void menu_play_music();
void sound_update();
void level_select_music();
void level_select_stop();
void game_music_play();
void game_music_stop();
void win_music_play();
void win_music_stop();
void lose_music_play();
void lose_music_stop();

void menu_stop();

#endif //MAIN_C_SOUNDS_H
