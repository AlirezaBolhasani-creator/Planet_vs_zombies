#include "plant.h"
#include "grid.h"
#include "raylib.h"

static PlantTextures g_tex;
void plant_set_textures(PlantTextures tex)
{
    // Textures are injected from game_init
    g_tex = tex;
}

static const float NORMAL_COVER  = 1.0f;
static const float PREVIEW_SCALE = 0.85f;

static void draw_plant_tex_tinted(Texture2D tex, int x, int y, float cover, Color tint)
{
    // Fit texture inside tile while keeping aspect ratio
    float maxDim = (float)(TILE_SIZE) * cover;
    float s1 = maxDim / (float)tex.width;
    float s2 = maxDim / (float)tex.height;
    float scale = (s1 < s2) ? s1 : s2;

    float w = (float)tex.width * scale;
    float h = (float)tex.height * scale;

    Rectangle src = {0, 0, (float)tex.width, (float)tex.height };
    Rectangle dst = {(float)x + TILE_SIZE/2.0f, (float)y + TILE_SIZE/2.0f, w, h };
    Vector2 origin = { w/2.0f, h/2.0f };

    DrawTexturePro(tex, src, dst, origin, 0.0f, tint);
}

static void draw_plant_tex(Texture2D tex, int x, int y, float cover)
{
    // Normal draw (no tint)
    draw_plant_tex_tinted(tex, x, y, cover, WHITE);
}

void plant_grid_clear(Plant plants[GRID_ROWS][GRID_COLS])
{
    // Reset all cells to empty
    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++)
        {
            plants[r][c].type = PLANT_NONE;
            plants[r][c].alive = false;
            plants[r][c].row = r;
            plants[r][c].col = c;
        }
}

//UI bars (HP + lifetime)
static float clamp01(float x)
{
    // Clamp for bar ratios
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x;
}

static void draw_thin_bar(int x, int y, int w, int h, float ratio, Color fill, Color border, Color back)
{
    // Thin bar with fixed border (progress shrinks inside)
    ratio = clamp01(ratio);

    Rectangle bg   = { (float)x, (float)y, (float)w, (float)h };
    Rectangle prog = { (float)x, (float)y, (float)(w * ratio), (float)h };

    DrawRectangleRec(bg, back);
    if (ratio > 0.0f) DrawRectangleRec(prog, fill);
    DrawRectangleLinesEx(bg, 1, border);
}

static void plant_draw_bars(const Plant *p, int cellX, int cellY)
{
    // HP bar always, lifetime bar فقط برای Rose/Chomper
    if (!p->alive || p->type == PLANT_NONE) return;

    const int w = 54, h = 5;
    const int padX = (TILE_SIZE - w) / 2;

    int x = cellX + padX;
    int y = cellY + 6;

    float hpRatio = (p->maxHealth > 0) ? ((float)p->health / (float)p->maxHealth) : 0.0f;

    Color back   = Fade(BLACK, 0.20f);
    Color border = Fade(BLACK, 0.55f);

    draw_thin_bar(x, y, w, h, hpRatio, Fade(GREEN, 0.75f), border, back);

    if ((p->type == PLANT_ROSE || p->type == PLANT_CHOMPER) && p->lifeTime > 0.0f)
    {
        float lifeRatio = p->lifeTimer / p->lifeTime;
        draw_thin_bar(x, y + h + 3, w, h, lifeRatio, Fade(ORANGE, 0.75f), border, back);
    }
}

void plant_grid_draw(const Plant plants[GRID_ROWS][GRID_COLS])
{
    // Draw plants per cell + UI bars
    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++)
        {
            plant_type t = plants[r][c].type;
            if (t == PLANT_NONE) continue;
            int x = ORIGIN_X + c * TILE_SIZE;
            int y = ORIGIN_Y + r * TILE_SIZE;
            switch (t)
            {
                case PLANT_PEA:       draw_plant_tex(g_tex.pea, x, y, NORMAL_COVER); break;
                case PLANT_SUNFLOWER: draw_plant_tex(g_tex.sunflower, x, y, NORMAL_COVER); break;
                case PLANT_ROSE:      draw_plant_tex(g_tex.rose, x, y, NORMAL_COVER); break;
                case PLANT_CHOMPER:   draw_plant_tex(g_tex.chomper_tex, x, y, NORMAL_COVER); break;
                case PLANT_ICE_PEA:   draw_plant_tex(g_tex.ice_pea, x, y, NORMAL_COVER);break;

                default: break;
            }
            plant_draw_bars(&plants[r][c], x, y);
        }
}
bool plant_cell_is_empty(const Plant plants[GRID_ROWS][GRID_COLS], int col, int row)
{
    // Grid occupancy check
    if (!grid_in_bounds(col, row)) return false;
    return plants[row][col].type == PLANT_NONE;
}

bool plant_place(Plant plants[GRID_ROWS][GRID_COLS], plant_type t, int col, int row)
{
    // Place plant only on empty valid cell
    if (!grid_in_bounds(col, row)) return false;
    if (plants[row][col].type != PLANT_NONE) return false;

    plants[row][col].row = row;
    plants[row][col].col = col;
    plant_init_cel(&plants[row][col], t);
    return true;
}
void plant_draw_preview(Plant plants[GRID_ROWS][GRID_COLS], plant_type t, int col, int row, Color tint)
{
    // Ghost preview on empty cell
    if (!grid_in_bounds(col, row)) return;
    if (plants[row][col].type != PLANT_NONE) return;

    int x = ORIGIN_X + col * TILE_SIZE;
    int y = ORIGIN_Y + row * TILE_SIZE;

    float previewCover = NORMAL_COVER * PREVIEW_SCALE;

    switch (t)
    {
        case PLANT_PEA:       draw_plant_tex_tinted(g_tex.pea, x, y, previewCover, tint); break;
        case PLANT_SUNFLOWER: draw_plant_tex_tinted(g_tex.sunflower, x, y, previewCover, tint); break;
        case PLANT_ROSE:      draw_plant_tex_tinted(g_tex.rose, x, y, previewCover, tint); break;
        case PLANT_CHOMPER:   draw_plant_tex_tinted(g_tex.chomper_tex, x, y, previewCover, tint); break;
        //new
        case PLANT_ICE_PEA:   draw_plant_tex_tinted(g_tex.ice_pea, x, y, previewCover, tint); break;
        default: break;
    }
}

void plant_init_cel(Plant *p, plant_type t)
{
    // Default stats per plant type
    p->type = t;
    p->alive = (t != PLANT_NONE);

    if (t == PLANT_PEA)
    {
        p->maxHealth = p->health = 100;
        p->cooldown = 5.0f;
        p->cooldownTimer = p->cooldown;
        p->actionInterval = 3.0f;
        p->actionTimer = p->actionInterval;
    }
    else if (t == PLANT_SUNFLOWER)
    {
        p->maxHealth = p->health = 80;
        p->cooldownTimer = p->cooldown = 5.0f;
        p->actionInterval = 7.0f;
        p->actionTimer = p->actionInterval;
    }
    else if (t == PLANT_ROSE)
    {
        p->maxHealth = p->health = 100;
        p->cooldownTimer = p->cooldown = 5.0f;
        p->actionInterval = p->actionTimer = 0.0f;
        p->lifeTime = 10.0f;
        p->lifeTimer = p->lifeTime;
    }
    else if (t == PLANT_CHOMPER)
    {
        p->maxHealth = p->health = 150;
        p->cooldownTimer = p->cooldown = 10.0f;
        p->actionInterval = 0.0f;
        p->actionTimer = 0.0f;
        p->lifeTime = 20.0f;
        p->lifeTimer = p->lifeTime;
    }
    else if(t == PLANT_ICE_PEA)
    {
        p->maxHealth = p->health = 100;
        p->cooldown = 5.0f;
        p->cooldownTimer = p->cooldown;
        p->actionInterval = 3.0f;   // مثل pea
        p->actionTimer = p->actionInterval;
    }
    else
    {
        p->maxHealth = p->health = 0;
        p->cooldownTimer = p->cooldown = 0.0f;
        p->actionInterval = p->actionTimer = 0.0f;
    }
}
int plant_collect_events(Plant plants[GRID_ROWS][GRID_COLS], float dt, PlantEvent* out_events, int max_events)
{
    // Update timers and output events (bullet/sun) to be handled in game.c
    int count = 0;
    for (int row = 0; row < GRID_ROWS; row++)
        for (int col = 0; col < GRID_COLS; col++)
        {
            Plant *p = &plants[row][col];
            if (p->type == PLANT_NONE || !p->alive) continue;

            if (p->health <= 0) { plant_init_cel(p, PLANT_NONE); continue; }

            if (p->actionInterval > 0.0f)
            {
                p->actionTimer -= dt;
                if (p->actionTimer <= 0.0f)
                {
                    if (count < max_events)
                    {
                        if (p->type == PLANT_PEA || p->type == PLANT_ICE_PEA) out_events[count++] = (PlantEvent){ EVENT_SPAWN_BULLET, row, col };
                        else if (p->type == PLANT_SUNFLOWER) out_events[count++] = (PlantEvent){ EVENT_SPAWN_SUN, row, col };
                    }
                    p->actionTimer = p->actionInterval;
                }
            }

            if (p->type == PLANT_CHOMPER || p->type == PLANT_ROSE)
            {
                p->lifeTimer -= dt;
                if (p->lifeTimer <= 0.0f) plant_init_cel(p, PLANT_NONE);
            }
        }

    return count;
}
void apply_rose_effects(Plant plants[GRID_ROWS][GRID_COLS], float dt, float rose_heal_acc[GRID_ROWS])
{
    // Row-based aura: pea fire-rate buff + heal per second
    bool rowHasRose[GRID_ROWS] = {0};

    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++)
            if (plants[r][c].alive && plants[r][c].type == PLANT_ROSE) { rowHasRose[r] = true; break; }

    const float peaBaseInterval = 1.0f;

    for (int r = 0; r < GRID_ROWS; r++)
    {
        float newInterval = rowHasRose[r] ? peaBaseInterval * 0.8f : peaBaseInterval;

        for (int c = 0; c < GRID_COLS; c++)
        {
            Plant *p = &plants[r][c];
            if (!p->alive || p->type != PLANT_PEA || p->type != PLANT_ICE_PEA) continue; //new if

            if (p->actionInterval > 0.0f && p->actionInterval != newInterval)
            {
                float old = p->actionInterval;
                p->actionInterval = newInterval;
                p->actionTimer *= (newInterval / old);

                if (p->actionTimer > p->actionInterval) p->actionTimer = p->actionInterval;
                if (p->actionTimer < 0.01f) p->actionTimer = 0.01f;
            }
            else p->actionInterval = newInterval;
        }
    }
    for (int r = 0; r < GRID_ROWS; r++)
    {
        if (!rowHasRose[r]) { rose_heal_acc[r] = 0.0f; continue; }

        rose_heal_acc[r] += dt;
        while (rose_heal_acc[r] >= 1.0f)
        {
            rose_heal_acc[r] -= 1.0f;
            for (int c = 0; c < GRID_COLS; c++)
            {
                Plant *p = &plants[r][c];
                if (!p->alive || p->type == PLANT_NONE || p->type == PLANT_ROSE) continue;

                int add = (int)ceilf(0.10f * (float)p->maxHealth);
                p->health += add;
                if (p->health > p->maxHealth) p->health = p->maxHealth;
            }
        }
    }
}