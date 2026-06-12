#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "game.h"
int main()
{
    InitWindow(1200, 900, "PVZ");
    ToggleFullscreen();
    SetTargetFPS(120);
    enum game_state game = STATE_MENU;
    game_init();

    srand(time(NULL));

    while (!WindowShouldClose() && game != STATE_EXIT)
    {
        float dt = GetFrameTime();
        if (dt > 0.1f) dt = 0.1f;

        game_update(&game, dt);
        BeginDrawing();
        game_draw(&game);
        EndDrawing();
    }

    game_unload();
    CloseWindow();
    return 0;
}