#include "sounds.h"
static Music menu_music, choose_your_seed, game_music, win_music, lose_music;
static Sound sfx_plant, sfx_throw, sfx_splat, sfx_points, sfx_zombie_die, sfx_mower, sfx_click;
void init_sounds()
{
    InitAudioDevice();
    menu_music = LoadMusicStream(ASSET("Sounds//Menu.mp3"));
    menu_music.looping = true;
    choose_your_seed = LoadMusicStream(ASSET("Sounds//Choose_Level.mp3"));
    choose_your_seed.looping = true;
    game_music = LoadMusicStream(ASSET("Sounds//Grasswalk IN-GAME.mp3"));
    game_music.looping = true;
    win_music = LoadMusicStream(ASSET("Sounds//winmusic.mp3"));
    win_music.looping = false;
    lose_music = LoadMusicStream(ASSET("Sounds//losemusic.mp3"));
    lose_music.looping = false;

    sfx_plant      = LoadSound(ASSET("Sounds//SFX plant.wav"));
    sfx_throw      = LoadSound(ASSET("Sounds//SFX throw.wav"));
    sfx_splat      = LoadSound(ASSET("Sounds//SFX splat.wav"));
    sfx_points     = LoadSound(ASSET("Sounds//SFX points.wav"));
    sfx_zombie_die = LoadSound(ASSET("Sounds//SFX zombie falling 2.wav"));
    sfx_mower      = LoadSound(ASSET("Sounds//SFX lawnmower.wav"));
    sfx_click      = LoadSound(ASSET("Sounds//SFX buttonclick.wav"));

}
void menu_play_music(void)
{
    if (!IsMusicStreamPlaying(menu_music))
        PlayMusicStream(menu_music);
}
void level_select_music()
{
    if(!IsMusicStreamPlaying(choose_your_seed))
        PlayMusicStream(choose_your_seed);
}
void game_music_play()
{
    if(!IsMusicStreamPlaying(game_music))
        PlayMusicStream(game_music);
}
void game_music_stop()
{
    StopMusicStream(game_music);
}
void win_music_play()
{
    if(!IsMusicStreamPlaying(win_music))
        PlayMusicStream(win_music);
}
void win_music_stop()
{
    StopMusicStream(win_music);
}
void lose_music_play()
{
    if(!IsMusicStreamPlaying(lose_music))
        PlayMusicStream(lose_music);
}
void lose_music_stop()
{
    StopMusicStream(lose_music);
}
void sound_update()
{
    UpdateMusicStream(menu_music);
    UpdateMusicStream(choose_your_seed);
    UpdateMusicStream(game_music);
    UpdateMusicStream(win_music);
    UpdateMusicStream(lose_music);

}
void menu_stop()
{
    StopMusicStream(menu_music);
}
void level_select_stop()
{
    StopMusicStream(choose_your_seed);
}
void play_sfx(SfxId id)
{
    switch(id)
    {
        case SFX_PLANT: PlaySound(sfx_plant); break;
        case SFX_THROW: PlaySound(sfx_throw); break;
        case SFX_SPLAT: PlaySound(sfx_splat); break;
        case SFX_POINTS: PlaySound(sfx_points); break;
        case SFX_ZOMBIE_DIE: PlaySound(sfx_zombie_die); break;
        case SFX_MOWER: PlaySound(sfx_mower); break;
        case SFX_CLICK: PlaySound(sfx_click); break;
        case SFX_ERROR: /* PlaySound(sfx_error); */ break;
    }
}
void unload_sounds()
{
    UnloadMusicStream(menu_music);
    UnloadMusicStream(choose_your_seed);
    UnloadMusicStream(game_music);
    UnloadMusicStream(win_music);
    UnloadMusicStream(lose_music);
    UnloadSound(sfx_click);
    UnloadSound(sfx_mower);
    UnloadSound(sfx_plant);
    UnloadSound(sfx_points);
    UnloadSound(sfx_throw);
    UnloadSound(sfx_splat);
    UnloadSound(sfx_zombie_die);
    CloseAudioDevice();
}