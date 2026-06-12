#include "zombie.h"
#include "animation.h"
// برای زامبی متفکر: هر بار که وارد ستون جدید میشه تصمیم می‌گیره ردیف عوض کنه
static int last_col[MAX_ZOMBIES];

// انیمیشن‌های مختلف زامبی (نرمال/پرچمی = متفکر)
static Animation zombieWalkAnim;
static Animation zombieHitAnim;
static Animation zombieDieAnim;
static Animation zombieFlagWalkAnim;
static Animation zombieFlagHitAnim;
// انتخاب state مناسب بر اساس نوع زامبی (نرمال یا متفکر)
static ZombieAnimState pick_walk_state(const Zombie* z)
{
    return (z->type == ZOMBIE_THINKING) ? ZOMBIE_FLAG_WALK : ZOMBIE_WALK;
}
static ZombieAnimState pick_hit_state(const Zombie* z)
{
    return (z->type == ZOMBIE_THINKING) ? ZOMBIE_FLAG_HIT : ZOMBIE_HIT;
}
// بارگذاری همه‌ی sprite sheet ها برای انیمیشن‌ها
void zombies_load_animations()
{
    zombieWalkAnim     = Animation_Load(ASSET("zombie_walk.png"),41, 56, 7, 12, true, 1);
    zombieHitAnim      = Animation_Load(ASSET("zombie_eat.png"),41, 56, 7, 10, true, 1);
    zombieDieAnim      = Animation_Load(ASSET("zombie_fall.png"),41, 56, 5, 8,  false, 1);
    zombieFlagHitAnim  = Animation_Load(ASSET("zombie_flag_eat.png"),41, 56, 7, 12, true, 1);
    zombieFlagWalkAnim = Animation_Load(ASSET("zombie_flag_walk.png"),41, 56, 7, 10, true, 1);
}

// آزاد کردن تکسچرهای انیمیشن‌ها
void zombies_unload_animations()
{
    Animation_Unload(&zombieWalkAnim);
    Animation_Unload(&zombieHitAnim);
    Animation_Unload(&zombieFlagHitAnim);
    Animation_Unload(&zombieDieAnim);
    Animation_Unload(&zombieFlagWalkAnim);
}

// نسبت سلامت گیاه (برای تصمیم‌گیری زامبی متفکر)
static float plant_ratio(const Plant* p)
{
    if(!p->alive || p->maxHealth <= 0.0f) return 0.0f;
    return (float)p->health / (float)p->maxHealth;
}

// وزن هر ردیف: هرچی کمتر باشه => ردیف “ضعیف‌تر” و هدف بهتر برای زامبی متفکر
// نکته: mower_term باعث میشه اگر موور وجود داشته باشه (حتی اگر فعال نیست) وزن زیاد بشه و زامبی از اون ردیف دوری کنه
static float row_weight(int row, const Plant plants[GRID_ROWS][GRID_COLS], const Mower mowers[GRID_COLS])
{
    int n = 0;
    float s = 0.0f, q = 0.0f;

    for(int c = 0; c < GRID_COLS; c++)
    {
        const Plant* p = &plants[row][c];

        if(p->alive && p->type != PLANT_NONE && p->type != PLANT_ROSE && p->type != PLANT_SUNFLOWER)
        {
            float r = plant_ratio(p);
            s += r;
            q += r * r;
            n++;
        }
    }
    float det = 1.0f + (float)(n*n) + (s*s) + (q*q);

    float mower_term = 0.0f;
    if(mowers[row].active || mowers[row].available) mower_term = 1.0f;

    return 10 * mower_term + det;
}

// زامبی متفکر فقط بین سه گزینه تصمیم می‌گیره: ردیف بالا / همین ردیف / ردیف پایین
static int thinking_choose_row(int currentRow, const Plant plants[GRID_ROWS][GRID_COLS], const Mower mowers[GRID_ROWS])
{
    int candidates[3] = { currentRow-1, currentRow, currentRow+1 };

    int bestRow = currentRow;
    float bestW = row_weight(currentRow, plants, mowers);

    for(int i = 0; i < 3; i++)
    {
        if(candidates[i] < 0 || candidates[i] >= GRID_ROWS) continue;
        float w = row_weight(candidates[i], plants, mowers);

        if(w < bestW)
        {
            bestRow = candidates[i];
            bestW = w;
        }
    }
    return bestRow;
}
static float zombie_start_x(void)
{
    return ORIGIN_X + GRID_COLS * TILE_SIZE + 50.0f;
}

// ریست اولیه‌ی آرایه زامبی‌ها
void zombies_init(Zombie* zombies, int count)
{
    for (int i = 0; i < count; i++)
    {
        zombies[i].active = false;
        last_col[i] = -1;

        zombies[i].type = ZOMBIE_NORMAL;
        zombies[i].animState = ZOMBIE_WALK;
        zombies[i].anim = zombieWalkAnim;
        Animation_Reset(&zombies[i].anim);

        zombies[i].die_sfx_played = false;
    }
}

// تغییر state و انتخاب انیمیشن مرتبط
void zombie_set_state(Zombie* z, ZombieAnimState newState)
{
    if (z->animState == newState) return;
    z->animState = newState;

    switch (newState)
    {
        case ZOMBIE_WALK:      z->anim = zombieWalkAnim;     break;
        case ZOMBIE_HIT:       z->anim = zombieHitAnim;      break;
        case ZOMBIE_DIE:       z->anim = zombieDieAnim;      break;
        case ZOMBIE_FLAG_HIT:  z->anim = zombieFlagHitAnim;  break;
        case ZOMBIE_FLAG_WALK: z->anim = zombieFlagWalkAnim; break;
    }
    Animation_Reset(&z->anim);
}
// تبدیل x به شماره ستون (برای چک کردن برخورد با گیاه)
static int x_to_col(float x)
{
    int col = (int)floorf((x - ORIGIN_X) / (float)TILE_SIZE);
    if(col <= 0) return 0;
    if(col >= GRID_COLS) return GRID_COLS - 1;
    return col;
}
// اسپاون زامبی نرمال در ردیف تصادفی
void zombie_spawn_random_row(Zombie* zombies, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (!zombies[i].active)
        {
            zombies[i].x = zombie_start_x();
            zombies[i].active = true;

            zombies[i].speed  = 20.0f;
            zombies[i].health = 100;
            zombies[i].row    = rand() % GRID_ROWS;

            zombies[i].attackInterval = 0.8f;
            zombies[i].attackTimer    = zombies[i].attackInterval;
            zombies[i].damage         = 20;

            zombies[i].type = ZOMBIE_NORMAL;
            zombie_set_state(&zombies[i], ZOMBIE_WALK);
            zombies[i].die_sfx_played = false;
            //new
            zombies[i].baseSpeed = 30.0f;
            zombies[i].speed = zombies[i].baseSpeed;
            zombies[i].slowTimer = 0.0f;

            break;
        }
    }
}
// آپدیت اصلی: حرکت، حمله، مرگ، و تصمیم‌گیری زامبی متفکر
void zombies_update(Zombie* zombies, int count, float dt,
                    Plant plants[GRID_ROWS][GRID_COLS],
                    bool* game_over, float rowMul[GRID_ROWS],
                    const Mower mowers[GRID_ROWS])
{
    for (int i = 0; i < count; i++)
    {
        Zombie* z = &zombies[i];
        if (!z->active) continue;

        int col = x_to_col(z->x);
        int row = z->row;
        bool hasPlantAhead = !plant_cell_is_empty(plants, col, row);

        // slow effect update
        if (z->slowTimer > 0.0f)
        {
            z->slowTimer -= dt;
            if (z->slowTimer < 0.0f) z->slowTimer = 0.0f;
        }
        // سرعت موثر: اگر اسلو فعال باشد نصف
        float effectiveSpeed = z->baseSpeed * ((z->slowTimer > 0.0f) ? 0.5f : 1.0f);

        // 1) اگر مرده: انیمیشن مرگ و بعد حذف
        if (z->health <= 0)
        {
            if(!z->die_sfx_played)
            {
                play_sfx(SFX_ZOMBIE_DIE);
                z->die_sfx_played = true;
            }

            zombie_set_state(z, ZOMBIE_DIE);
            Animation_Update(&z->anim, dt);

            if(z->anim.finished) z->active = false;
            continue;
        }

        // 2) انتخاب انیمیشن بر اساس وضعیت (راه رفتن / خوردن)
        if (hasPlantAhead) zombie_set_state(z, pick_hit_state(z));
        else              zombie_set_state(z, pick_walk_state(z));

        Animation_Update(&z->anim, dt);

        // 3) قبل از ورود به زمین اصلی فقط حرکت می‌کند
        if (z->x > ORIGIN_X + GRID_COLS * TILE_SIZE)
        {
            z->x -= effectiveSpeed * dt * rowMul[z->row];
            continue;
        }

        if (z->x < ORIGIN_X - 10.0f)
        {
            z->active = false;
            *game_over = true;
            continue;
        }

        // 5) اگر جلویش گیاه هست: با فاصله‌ی زمانی damage می‌زند
        if (!plant_cell_is_empty(plants, col, row))
        {
            z->attackTimer -= dt;
            if (z->attackTimer <= 0.0f)
            {
                plants[row][col].health -= z->damage;
                z->attackTimer = z->attackInterval;

                if (plants[row][col].health <= 0)
                    plant_init_cel(&plants[row][col], PLANT_NONE);
            }
        }
        else
        {
            //  اگر راه بازه: حرکت + (فقط متفکر) هر ستون یکبار تصمیم تغییر ردیف
            int col_now = x_to_col(z->x);

            if (z->type == ZOMBIE_THINKING)
            {
                if (last_col[i] != col_now)
                {
                    z->row = thinking_choose_row(z->row, plants, mowers);
                    last_col[i] = col_now;
                }
            }
            else last_col[i] = col_now;

            z->x -= effectiveSpeed * dt * rowMul[z->row];
            z->attackTimer = z->attackInterval;
        }
    }
}

// رسم زامبی‌ها (با Animation_Draw)
void zombies_draw(const Zombie* zombies, int count)
{
    for (int i = 0; i < count; i++)
    {
        const Zombie* z = &zombies[i];
        if(!z->active) continue;

        Vector2 pos = { z->x, ORIGIN_Y + z->row * TILE_SIZE };
        Animation_Draw((Animation*)&z->anim, pos, 1.5f);
    }
}
// اسپاون زامبی متفکر در ردیف تصادفی
void zombie_spawn_thinking_random_row(Zombie* zombies, int count)
{
    int index = -1;
    for (int i = 0; i < count; i++)
    {
        if (!zombies[i].active) { index = i; break; }
    }
    if (index < 0) return;

    int row = rand() % GRID_ROWS;

    Zombie* z = &zombies[index];

    z->x      = zombie_start_x();
    z->row    = row;
    z->active = true;

    z->health = 100;
    z->type   = ZOMBIE_THINKING;

    z->attackInterval = 0.8f;
    z->attackTimer    = z->attackInterval;
    z->damage         = 20;

    z->baseSpeed = 20.0f;
    z->slowTimer = 0.0f;
    z->speed     = z->baseSpeed;

    z->die_sfx_played = false;

    last_col[index] = -1;

    z->animState = -999;
    zombie_set_state(z, ZOMBIE_FLAG_WALK);
}

