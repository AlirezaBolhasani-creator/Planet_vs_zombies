#include "raylib.h"
#include "ui_menu.h"
static Rectangle playbtn = {260, 490, 190,110};
static Rectangle shopbtn = {500, 490, 190, 110};
static Rectangle exitbtn = {740, 490, 190, 110};
menu_res menu_update()
{
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if(CheckCollisionPointRec(mouse, playbtn))
            return menu_play;
        if(CheckCollisionPointRec(mouse, shopbtn))
            return menu_shop;
        if(CheckCollisionPointRec(mouse, exitbtn))
            return menu_exit;
    }
    return menu_none;
}
void menu_show(Texture2D background) {

    float screenW = (float)GetScreenWidth();
    float screenH = (float)GetScreenHeight();
    float imageW  = (float)background.width;
    float imageH  = (float)background.height;

    float scale = (screenW / imageW > screenH / imageH) ? screenW / imageW : screenH / imageH;

    float finalW = imageW * scale;
    float finalH = imageH * scale;

    float offsetX = (screenW - finalW) / 2;
    float offsetY = (screenH - finalH) / 2;

    DrawTexturePro(background,
                   (Rectangle){0,0, imageW, imageH},
                   (Rectangle){offsetX, offsetY, finalW, finalH},
                   (Vector2){0,0}, 0.0f, WHITE);

    Vector2 mouse = GetMousePosition();

    Color playColor = CheckCollisionPointRec(mouse, playbtn) ? RED : BLACK;
    Color shopColor = CheckCollisionPointRec(mouse, shopbtn) ? RED : BLACK;
    Color exitColor = CheckCollisionPointRec(mouse, exitbtn) ? RED : BLACK;

    DrawText("Play", playbtn.x + 50, playbtn.y + 30, 40, playColor);

    DrawText("Shop", shopbtn.x + 50, shopbtn.y + 30, 40, shopColor);

    DrawText("Exit", exitbtn.x + 50, exitbtn.y + 30, 40, exitColor);
}