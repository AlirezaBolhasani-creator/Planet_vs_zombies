#include "raylib.h"
#include "levels.h"

static Rectangle level_one_btn = {200, 70, 950,130};
static Rectangle level_two_btn = {200, 270, 950, 130};
static Rectangle level_three_btn = {200, 470, 950, 130};
static Rectangle level_four_btn = {200, 680, 950, 130};

level_res update_level()
{
    Vector2 mouse = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if(CheckCollisionPointRec(mouse,level_one_btn))
            return level_one;

        else if(CheckCollisionPointRec(mouse,level_two_btn))
            return level_two;

        else if(CheckCollisionPointRec(mouse, level_three_btn))
            return level_three;

        else if(CheckCollisionPointRec(mouse,level_four_btn))
            return level_four;
    }
    return level_none;
}
void level_show(Texture2D level_picture)
{
    float screenW = (float)GetScreenWidth();
    float screenH = (float)GetScreenHeight();
    float imageW  = (float)level_picture.width;
    float imageH  = (float)level_picture.height;

    float scale = (screenW / imageW > screenH / imageH) ? screenW / imageW : screenH / imageH;

    float finalW = imageW * scale;
    float finalH = imageH * scale;

    float offsetX = (screenW - finalW) / 2;
    float offsetY = (screenH - finalH) / 2;

    DrawTexturePro(level_picture,
                   (Rectangle){0,0, imageW, imageH},       // کل تصویر منبع
                   (Rectangle){offsetX, offsetY, finalW, finalH},  // مقصد
                   (Vector2){0,0}, 0.0f, WHITE);

    Vector2 mouse = GetMousePosition();

    Color level_one_color = CheckCollisionPointRec(mouse, level_one_btn)? RED : BLACK;
    Color level_two_color = CheckCollisionPointRec(mouse, level_two_btn)? RED : BLACK;
    Color level_three_color = CheckCollisionPointRec(mouse, level_three_btn)? RED : BLACK;
    Color level_four_color = CheckCollisionPointRec(mouse, level_four_btn)? RED : BLACK;
    DrawText("Level One", level_one_btn.x + 20,  level_one_btn.y + 25, 60, level_one_color);

    DrawText("Level Two", level_two_btn.x + 20, level_two_btn.y + 25, 60, level_two_color);

    DrawText("Level Three", level_three_btn.x + 20, level_three_btn.y + 25, 60, level_three_color);

    DrawText("Survival", level_four_btn.x + 20, level_four_btn.y + 25, 60, level_four_color);
}
