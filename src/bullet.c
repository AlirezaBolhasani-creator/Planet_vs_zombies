#include "bullet.h"
#include "raylib.h"

// غیرفعال کردن همه گلوله‌ها در شروع مرحله
void bullet_init(Bullet *bullets)
{
    for(int i = 0; i < MAX_BULLETS; i++)
        bullets[i].active = false;
}

// ایجاد گلوله جدید در اولین اسلات آزاد
void spawn_bullet(int row, int col,BulletType type, Bullet *bullets)
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        if(!bullets[i].active)
        {
            bullets[i].row = row;
            bullets[i].active = true;

            // شروع از جلوی گیاه
            bullets[i].x = ORIGIN_X + col * TILE_SIZE + TILE_SIZE * 0.8f;

            bullets[i].speed = 220.0f;
            bullets[i].damage = 20;
            bullets[i].type = type;
            break;
        }
    }
}

// حرکت گلوله‌ها و حذف در صورت خروج از صفحه
void bullet_update(Bullet *bullets, float dt)
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        if(!bullets[i].active) continue;

        bullets[i].x += bullets[i].speed * dt;

        if(bullets[i].x > ORIGIN_X + TILE_SIZE * GRID_COLS)
            bullets[i].active = false;
    }
}

// رسم گلوله‌ها
void draw_bullets(Bullet *bullets)
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i].active)
        {

            DrawCircle(
                    (int)bullets[i].x,
                    ORIGIN_Y + bullets[i].row * TILE_SIZE + TILE_SIZE/2 - 10,
                    6,
                    bullets[i].type == BULLET_ICE ? BLUE : GREEN
            );
        }
    }
}
