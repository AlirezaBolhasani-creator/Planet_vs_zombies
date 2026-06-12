#include "grid.h"

// مقداردهی اولیه گرید: پیش‌فرض همه خانه‌ها چمن روشن
void grid_init(Grid* g)
{
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            g->tiles[r][c] = TILE_GRASS_LIGHT;
        }
    }
}

// ساخت نقشه مرحله: وسط چمن شطرنجی، ستون اول خاک، ستون آخر آسفالت
void grid_make_checker_grass(Grid* g)
{
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 1; c < GRID_COLS - 1; c++) {
            g->tiles[r][c] = ((r + c) % 2 == 0) ? TILE_GRASS_LIGHT : TILE_GRASS_DARK;
        }
    }

    for (int r = 0; r < GRID_ROWS; r++) {
        g->tiles[r][0] = TILE_SOIL;                 // مسیر شروع (کاشت ممنوع)
    }

    for (int r = 0; r < GRID_ROWS; r++) {
        g->tiles[r][GRID_COLS - 1] = TILE_ASPHALT;  // مسیر ورود زامبی‌ها (کاشت ممنوع)
    }
}

// چک مرزها برای جلوگیری از دسترسی خارج آرایه
bool grid_in_bounds(int col, int row)
{
    return (col >= 0 && col < GRID_COLS &&
            row >= 0 && row < GRID_ROWS);
}

// گرفتن نوع خانه (اگر بیرون بود، مثل بلاک در نظر می‌گیریم)
TileType grid_get(const Grid* g, int col, int row)
{
    if (!grid_in_bounds(col, row)) return TILE_BLOCK;
    return g->tiles[row][col];
}

// تنظیم نوع خانه (فقط اگر داخل محدوده باشد)
void grid_set(Grid* g, int col, int row, TileType t)
{
    if (!grid_in_bounds(col, row)) return;
    g->tiles[row][col] = t;
}

// قانون کاشت: فقط روی چمن‌ها اجازه کاشت داریم
bool grid_can_place_plant(const Grid* g, int col, int row)
{
    TileType t = grid_get(g, col, row);
    if(t == TILE_GRASS_LIGHT || t == TILE_GRASS_DARK)
        return true;
    return false;
}

// رسم گرید با توجه به TileType هر خانه
void grid_draw(const Grid* g, Texture2D grass, Texture2D soil, Texture2D asphalt)
{
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {

            int x = ORIGIN_X + c * TILE_SIZE;
            int y = ORIGIN_Y + r * TILE_SIZE;

            TileType t = g->tiles[r][c];

            if (t == TILE_GRASS_LIGHT)      DrawTexture(grass, x, y, GREEN);
            else if (t == TILE_GRASS_DARK)  DrawTexture(grass, x, y, DARKGREEN);
            else if (t == TILE_SOIL)        DrawTexture(soil, x, y, GRAY);
            else if (t == TILE_ASPHALT)     DrawTexture(asphalt, x, y, BROWN);
            else if (t == TILE_BLOCK)       DrawRectangle(x, y, TILE_SIZE, TILE_SIZE, BLACK);
        }
    }
}

// تبدیل مختصات موس (پیکسل) به مختصات سلول (col,row) برای کاشت
bool grid_world_to_cell(int x, int y, int* col, int* row)
{
    x -= ORIGIN_X;
    y -= ORIGIN_Y;
    if(x < 0 || y < 0)
        return false;

    int c = x / TILE_SIZE;
    int r = y / TILE_SIZE;

    if(!grid_in_bounds(c,r))
        return false;

    *col = c;
    *row = r;
    return true;
}
