#include "mower.h"
#include "zombie.h"   // برای دسترسی به فیلدهای Zombie (x,row,active,health,...)
#include <math.h>

// چک می‌کنه آیا در این ردیف زامبی به محدوده "سمت چپ" نزدیک شده یا نه
// (همین شرط باعث فعال شدن mower میشه)
static bool existsZombieAtLeft(int row, Zombie* zombies, int zombieCount)
{
    float triggerX = ORIGIN_X + TILE_SIZE * 0.8f;

    for (int i = 0; i < zombieCount; i++)
    {
        if (!zombies[i].active) continue;
        if (zombies[i].row != row) continue;

        if (zombies[i].x <= triggerX) return true;
    }
    return false;
}

// وقتی mower حرکت می‌کنه، هر زامبی‌ای که به x mower نزدیک باشه رو “له” می‌کنه
static void killZombiesInRowTouchingX(int row, float mowerX, Zombie* zombies, int zombieCount)
{
    float hitRange = TILE_SIZE * 0.45f;

    for (int z = 0; z < zombieCount; z++)
    {
        if (!zombies[z].active) continue;
        if (zombies[z].row != row) continue;

        if (fabsf(mowerX - zombies[z].x) < hitRange)
        {
            zombies[z].health = 0;
            zombies[z].active = false;

            // SFX مرگ فقط یک بار پخش بشه
            if(!zombies[z].die_sfx_played)
            {
                play_sfx(ZOMBIE_DIE); // (اگر enum تو پروژه‌ات SFX_ZOMBIE_DIE هست، همینجا باید همونو بزنی)
                zombies[z].die_sfx_played = true;
            }
        }
    }
}

// مقداردهی اولیه mower هر ردیف: جای شروع کنار چپ گرید + وضعیت آماده بودن
void mowers_init(Mower mowers[GRID_ROWS])
{
    for (int r = 0; r < GRID_ROWS; r++)
    {
        mowers[r].row = r;

        mowers[r].x = ORIGIN_X + TILE_SIZE * 0.5f;
        mowers[r].y = ORIGIN_Y + r * TILE_SIZE + TILE_SIZE * 0.5f;

        mowers[r].active = false;
        mowers[r].available = true;
        mowers[r].speed = 200.0f;
        mowers[r].sfx_played = false;
    }
}

// منطق اصلی mower: اگر زامبی به چپ نزدیک شد => فعال، سپس حرکت و کشتن زامبی‌ها
void mowers_update(Mower mowers[GRID_ROWS], Zombie* zombies, int zombieCount, float dt)
{
    for (int r = 0; r < GRID_ROWS; r++)
    {
        if (!mowers[r].available && !mowers[r].active) continue;
        if (!mowers[r].active)
        {
            if (existsZombieAtLeft(r, zombies, zombieCount))
            {
                mowers[r].active = true;
                mowers[r].sfx_played = false;
            }
        }

        // مرحله اجرا: حرکت به راست + برخورد/کشتن + تمام شدن
        if (mowers[r].active)
        {
            if (!mowers[r].sfx_played)
            {
                play_sfx(SFX_MOWER);
                mowers[r].sfx_played = true;
            }

            mowers[r].x += mowers[r].speed * dt;

            killZombiesInRowTouchingX(r, mowers[r].x, zombies, zombieCount);

            // وقتی از صفحه/گرید خارج شد => مصرف شده و برای همیشه خاموش
            if (mowers[r].x > ORIGIN_X + GRID_COLS * TILE_SIZE + TILE_SIZE)
            {
                mowers[r].active = false;
                mowers[r].available = false;
            }
        }
    }
}

// رسم mowerها: فقط mowerهایی که هنوز available هستند نمایش داده میشن
void mowers_draw(const Mower mowers[GRID_ROWS], Texture2D tex)
{
    for (int r = 0; r < GRID_ROWS; r++)
    {
        if (!mowers[r].available) continue;

        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
        Rectangle dst = {mowers[r].x, mowers[r].y, (float)TILE_SIZE, (float)TILE_SIZE};
        Vector2 origin = {TILE_SIZE / 2.0f, TILE_SIZE / 2.0f};
        DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
    }
}