#include "sun.h"
#include <stdlib.h>

void draw_sun(int y, int col, Texture2D sun)
{
    // رسم خورشید داخل سلول (با کمی شفافیت)
    int x = col * TILE_SIZE + ORIGIN_X;
    y += ORIGIN_Y;

    Rectangle src = {0, 0, sun.width, sun.height};
    Rectangle dst = { (float)x + TILE_SIZE / 4.0f, (float)y + TILE_SIZE / 4.0f,
                      (float)TILE_SIZE, (float)TILE_SIZE / 2 };
    DrawTexturePro(sun, src, dst, (Vector2){0,0}, 0, (Color){255, 255, 255, 200});
}

void click_on_sun(int* point_sun)
{
    // جمع کردن خورشید => اضافه شدن امتیاز + پخش صدا
    *point_sun += each_point;
    play_sfx(SFX_POINTS);
}

void spawn_sun_random(Sun *suns)
{
    // ساخت یک خورشید رندوم (مثل آسمون) و شروع سقوط
    for (int i = 0; i < MAX_SUNS; ++i)
    {
        if (!suns[i].active)
        {
            suns[i].active   = true;
            suns[i].lifeTime = SUN_LIFETIME;

            suns[i].y = 0;
            suns[i].row = rand() % GRID_ROWS;
            suns[i].col = rand() % (GRID_COLS - 2) + 1; // ستون‌های وسط (نه خاک/نه آسفالت)

            suns[i].move = true;
            suns[i].speed = 400.0f;
            break;
        }
    }
}

void update_suns(float dt, float* sun_spawn_timer, Sun* suns)
{
    // سیستم اسپاون دوره‌ای + آپدیت سقوط و حذف شدن با تایمر
    *sun_spawn_timer += dt;
    if (*sun_spawn_timer >= SUN_SPAWN_INTERVAL)
    {
        *sun_spawn_timer = 0.0f;
        spawn_sun_random(suns);
    }
    for (int i = 0; i < MAX_SUNS; ++i)
    {
        if (!suns[i].active) continue;

        if (suns[i].move) suns[i].y += dt * suns[i].speed;

        // وقتی به مقصد (سطح ردیف) رسید، دیگه سقوط نکنه
        if (suns[i].y >= suns[i].row * TILE_SIZE)
        {
            suns[i].move = false;
            suns[i].y = suns[i].row * TILE_SIZE;
        }

        suns[i].lifeTime -= dt;
        if (suns[i].lifeTime <= 0.0f) suns[i].active = false;
    }
}

bool handle_sun_click(Vector2 mouse, Sun* suns, int* sun_points)
{
    // کلیک روی خورشیدها: اگر موس داخل rect بود => جمعش کن
    for (int i = 0; i < MAX_SUNS; ++i)
    {
        if (!suns[i].active) continue;

        int x = ORIGIN_X + suns[i].col * TILE_SIZE + TILE_SIZE / 4;
        int y = ORIGIN_Y + (int)suns[i].y + TILE_SIZE / 4;
        Rectangle rect = { (float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE / 2.0f };

        if (CheckCollisionPointRec(mouse, rect))
        {
            suns[i].active = false;
            click_on_sun(sun_points);
            return true;
        }
    }
    return false;
}

void draw_suns(Sun* suns, Texture2D sunTex)
{
    // رندر همه خورشیدهای فعال
    for (int i = 0; i < MAX_SUNS; ++i)
        if (suns[i].active) draw_sun(suns[i].y, suns[i].col, sunTex);
}

void spawn_sun_at(Sun *suns , int row, int col)
{
    // اسپاون خورشید روی یک گیاه (برای sunflower event)
    for (int i = 0; i < MAX_SUNS; i++)
    {
        if (!suns[i].active)
        {
            suns[i].active = true;
            suns[i].row = row;
            suns[i].col = col;

            suns[i].lifeTime = SUN_LIFETIME;

            suns[i].y = row * TILE_SIZE + ORIGIN_Y;

            suns[i].move = true;
            suns[i].speed = 400.0f;
            break;
        }
    }
}