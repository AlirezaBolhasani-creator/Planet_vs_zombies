#include "raylib.h"
#include "game.h"
#include "ui_menu.h"
#include "levels.h"
#include "grid.h"
#include "plant.h"
#include "ui_shop.h"
#include <stdio.h>
#include "shop.h"
#include "sun.h"
#include <math.h>
#include "bullet.h"
#include "zombie.h"
#include "mower.h"
#include "sounds.h"

static Texture2D background;
static Texture2D level_back;
static Texture2D grass;
static Texture2D soil;
static Texture2D asphalt;
static Texture2D sunflower;
static Texture2D rose;
static Texture2D pea;
static Texture2D chomper_tex;

static Texture2D shop_bar;
static Texture2D shop_rect;
static Texture2D shop_back;
static Texture2D forbidden;
static Texture2D sun;
static Texture2D thinking_zombie_tex;

static Grid map;
static Texture2D plants_use_shop[number_of_plants];
static Plant plants_shop[number_of_plants];
static bool level_map_ready = false;
static Plant plants[GRID_ROWS][GRID_COLS];
static int hover_col = -1 , hover_row = -1;
static plant_type selected_plant = PLANT_NONE;

static Texture2D all_plants[all_plants_num];//now 3 later 6
static bool is_bought[all_plants_num];

static plant_shop plantShop[all_plants_num];
static int sun_points = 50;   // امتیاز اولیه‌ی خورشید

static float cd_pea = 0.0f;
static float cd_sunflower = 0.0f;
static float cd_rose = 0.0f;
static float cd_chomper = 0.0f;

static Sun  suns[MAX_SUNS];
static float sun_spawn_timer = 0.0f;

static Bullet bullets[MAX_BULLETS];
static PlantEvent plant_events[MAX_PLANT_EVENTS];

static Zombie zombies[MAX_ZOMBIES];
static Texture2D zombie_tex;

static float zombie_spawn_timer = 0.0f;
static bool game_over;


//Wave system 1...
static int   wave = 1;
static float wave_timer = 0.0f;
static float spawn_interval = 3.0f;
static const int level1_total_zombies = 30;
static int level1_spawned = 0;

//Wave system 2...
static const int level2_total_zombies = 30;
static int level2_spawned = 0;
static bool first_time_level2 = false;

//Wave system 3...
static const int level3_total_normal = 15;
static const int level3_total_thinking = 15;
static int level3_spawned_normal = 0;
static int level3_spawned_thinking = 0;
static bool first_time_level3 = false;

//system level 4...
static bool first_time_level4 = false;
static float time_level = 0.0f;
static bool error_reading_file = false;
static bool error_saving_file = false;
static int best_time;
static char str_level4[100];

//mower
static Mower mowers[GRID_ROWS];
static Texture2D mower_tex;

//rose
static float rose_heal_acc[GRID_ROWS] = {0};   // تایمر heal برای هر ردیف
//sound
static enum game_state prev_state = -1;
static enum game_state last_state = -1;

//ICE-PEA
static Texture2D ice_pea_tex;
static float cd_ice_pea = 0.0f;


void game_init()
{
    sun_points = 50;
    background = LoadTexture(ASSET("last_menu.png"));
    level_back  = LoadTexture(ASSET("level_final_back.png"));
    grass = LoadTexture(ASSET("light_grass.png"));
    soil = LoadTexture(ASSET("light_grass.png"));
    asphalt = LoadTexture(ASSET("light_grass.png"));
    pea = LoadTexture(ASSET("PEA.png"));
    sunflower = LoadTexture(ASSET("SUNFLOWER.png"));
    rose = LoadTexture(ASSET("ROSE.png"));
    shop_bar = LoadTexture(ASSET("shop_bar.png"));
    shop_rect = LoadTexture(ASSET("shop_rect.png"));
    shop_back = LoadTexture(ASSET("shop_back.jpg"));
    forbidden = LoadTexture(ASSET("forbidden.png"));
    sun = LoadTexture(ASSET("SUN.png"));
    mower_tex = LoadTexture(ASSET("mower.png"));
    zombie_tex = LoadTexture(ASSET("zombie.png"));//change it later
    chomper_tex = LoadTexture(ASSET("chomper.png"));
    thinking_zombie_tex = LoadTexture(ASSET("thinking.png"));

    //ice-pea
    ice_pea_tex = LoadTexture(ASSET("ICE_PEA.png"));

    zombies_load_animations();
    zombies_init(zombies,MAX_ZOMBIES);

    sun_spawn_timer = 0.0f;
    for (int i = 0; i < MAX_SUNS; ++i)
    {
        suns[i].active   = false;
        suns[i].lifeTime = 0.0f;
        suns[i].row      = 0;
        suns[i].col      = 0;
    }
    all_plants[0] = pea; all_plants[1] = sunflower; all_plants[2] = rose;all_plants[3] = chomper_tex;
    is_bought[0] = true; is_bought[1] = true; is_bought[2] = false; is_bought[3] = true;
    plantShop[0].is_bought = true;plantShop[1].is_bought = true; plantShop[2].is_bought = false;
    PlantTextures tex = {pea, sunflower, rose, chomper_tex, ice_pea_tex};
    plant_set_textures(tex);

    plants_shop[0].type = PLANT_PEA;
    plants_shop[1].type = PLANT_SUNFLOWER;
    plants_shop[2].type = PLANT_CHOMPER;
    plants_shop[3].type = PLANT_ROSE;
    plants_shop[4].type = PLANT_ICE_PEA;

    plants_use_shop[0] = pea;
    plants_use_shop[1] = sunflower;
    plants_use_shop[2] = chomper_tex;
    plants_use_shop[3] = rose;
    plants_use_shop[4] = ice_pea_tex;


    bullet_init(bullets);
    mowers_init(mowers);
    game_over = false;

    init_sounds();

}
void game_unload()
{
    UnloadTexture(background);
    UnloadTexture(level_back);
    UnloadTexture(grass);
    UnloadTexture(soil);
    UnloadTexture(asphalt);
    UnloadTexture(pea);
    UnloadTexture(sunflower);
    UnloadTexture(rose);
    UnloadTexture(shop_bar);
    UnloadTexture(shop_rect);
    UnloadTexture(shop_back);
    UnloadTexture(sun);
    UnloadTexture(forbidden);
    UnloadTexture(zombie_tex);
    UnloadTexture(mower_tex);
    UnloadTexture(chomper_tex);
    UnloadTexture(thinking_zombie_tex);
    zombies_unload_animations();
    unload_sounds();
    UnloadTexture(ice_pea_tex);//new

}
void game_reset()
{
    time_level = 0.0f;
    level_map_ready = false;
    sun_points = 50;
    cd_rose = cd_pea = cd_sunflower = 0;
    plant_grid_clear(plants);
    bullet_init(bullets);
    zombies_init(zombies, MAX_ZOMBIES);
    sun_spawn_timer = 0.0f;
    zombie_spawn_timer = 0.0f;
    for(int i = 0; i < MAX_SUNS; i++)
    {
        suns[i].active = false;
        suns[i].row = 0;
        suns[i].col = 0;
        suns[i].lifeTime = 0;
    }
    selected_plant = PLANT_NONE;
    hover_col = hover_row = -1;
    wave = 1;
    spawn_interval = 3.0f;
    wave_timer = 0.0f;
    game_over = false;
    mowers_init(mowers);
    level1_spawned = 0;
    level2_spawned = 0;
    first_time_level2 = false;
    level3_spawned_normal = 0;
    level3_spawned_thinking = 0;
    first_time_level3 = false;
    best_time = 0;
    for (int r = 0; r < GRID_ROWS; r++) rose_heal_acc[r] = 0.0f;

}
void game_update(enum game_state *game, float dt) {
    bool state_changed = (*game != prev_state);
    if (state_changed)
    {
        last_state = prev_state;
        prev_state = *game;
    }
    sound_update();

    switch (*game)
    {
        case STATE_MENU :
        {
            menu_play_music();
            menu_res res = menu_update();
            if (res == menu_play)
            {
                *game = STATE_LEVEL_SELECT;
                menu_stop();
            }
            if (res == menu_shop)
            {
                *game = STATE_SHOP;
                menu_stop();
            }
            if (res == menu_exit)
            {
                *game = STATE_EXIT;
                menu_stop();
            }
        }break;
        case STATE_LEVEL_SELECT:
        {
            level_select_music();
            level_res levelRes = update_level();
            if(levelRes == level_one)
            {
                *game = STATE_LEVEL1;
                level_map_ready = false;
                level_select_stop();
            }
            else if(levelRes == level_two)
            {
                *game = STATE_LEVEL2;
                level_map_ready = false;
                level_select_stop();
            }
            else if(levelRes == level_three)
            {
                *game = STATE_LEVEL3;
                level_map_ready = false;
                level_select_stop();
            }
            else if(levelRes == level_four)
            {
                *game = STATE_LEVEL4;
                level_map_ready = false;
                level_select_stop();
            }
        }break;
        case STATE_LEVEL1:
        {
            if(IsKeyPressed(KEY_C))
            {
                *game = STATE_GAME_OVER;
            }

            game_music_play();
            time_level += dt;
            //Wave-Update...
            wave_timer += dt;
            if(wave_timer >= 20.0f)
            {
                wave++;
                wave_timer = 0.0f;
                spawn_interval = fmaxf(0.8f, spawn_interval - 0.3f);
            }
            update_suns(dt, &sun_spawn_timer, suns);
            if(level1_total_zombies > level1_spawned)
            {
                zombie_spawn_timer += dt;
                if(zombie_spawn_timer >= spawn_interval)
                {
                    bool has_free = false;
                    for(int i = 0; i < MAX_ZOMBIES; i++)
                    {
                        if(zombies[i].active == false){ has_free = true; break;}
                    }
                    if(has_free)
                    {
                        zombie_spawn_random_row(zombies, MAX_ZOMBIES);
                        level1_spawned++;
                    }
                    zombie_spawn_timer= 0.0f;
                }
            }
            //end_of_wave...
            float rowMul[GRID_ROWS];
            for(int r=0;r<GRID_ROWS;r++) rowMul[r]=1.0f;

            for(int r = 0; r < GRID_ROWS; r++)
            {
                for(int c = 0; c < GRID_COLS; c++)
                {
                    if(plants[r][c].type == PLANT_CHOMPER && plants[r][c].alive)
                    {
                        rowMul[r] = 1.0f/3.0f;
                        break;
                    }
                }
            }
            zombies_update(zombies, MAX_ZOMBIES, dt, plants, &game_over, rowMul, mowers);
            mowers_update(mowers,zombies, MAX_ZOMBIES, dt);
            cd_pea = fmaxf(0.0f, cd_pea - dt);
            cd_sunflower = fmaxf(0.0f, cd_sunflower - dt);
            cd_rose = fmaxf(0.0f, cd_rose - dt);
            cd_chomper = fmaxf(0.0f,cd_chomper - dt);
            cd_ice_pea = fmaxf(0.0f, cd_ice_pea - dt);//new


            apply_rose_effects(plants, dt, rose_heal_acc);

            int evCount = plant_collect_events(plants, dt, plant_events, MAX_PLANT_EVENTS);
            for(int i = 0; i < evCount; i++)
            {
                PlantEvent ev = plant_events[i];
                if(ev.type == EVENT_SPAWN_BULLET)
                {
                    for(int j = 0; j < MAX_ZOMBIES; j++)
                    {
                        int plantX = ORIGIN_X + ev.col * TILE_SIZE;
                        if(zombies[j].row == ev.row && zombies[j].active && zombies[j].x > plantX && zombies[j].x < 1100)
                        {
                            Plant *pp = &plants[ev.row][ev.col];
                            BulletType bt = (pp->type == PLANT_ICE_PEA) ? BULLET_ICE : BULLET_NORMAL;
                            spawn_bullet(ev.row, ev.col, bt, bullets);
                            play_sfx(SFX_THROW);
                            break;
                        }
                    }
                }
                else if(ev.type == EVENT_SPAWN_SUN)
                {
                    spawn_sun_at(suns, ev.row, ev.col);
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                int mc = -1, mr = -1;
                int x_loc = GetMouseX(), y_loc = GetMouseY();
                if(handle_sun_click((Vector2){(float )x_loc,(float )y_loc}, suns, &sun_points))
                {
                    break;
                }
                else if (grid_world_to_cell(x_loc, y_loc, &mc, &mr) && selected_plant != PLANT_NONE)
                {
                    if (grid_can_place_plant(&map, mc, mr))
                    {
                        int cost = PLANT_COST[selected_plant];

                        if (sun_points >= cost)
                        {
                            if (plant_place(plants, selected_plant, mc, mr))
                            {
                                sun_points -= cost;
                                play_sfx(SFX_PLANT);
                                if(selected_plant == PLANT_PEA) cd_pea = 5.0f;
                                else if(selected_plant == PLANT_SUNFLOWER) cd_sunflower = 5.0f;
                                else if(selected_plant == PLANT_ROSE) cd_rose = 5.0f;
                                else if(selected_plant == PLANT_CHOMPER) cd_chomper = 5.0f;
                                else if(selected_plant == PLANT_ICE_PEA) cd_ice_pea = 5.0f;

                                selected_plant = PLANT_NONE;
                            }
                        }
                        else
                        {
                            // این بخش فقط برای دیباگ و تست:
                            printf("Not enough sun for this plant!\n");
                            // اگر خواستی، می‌تونی بعداً کنار موس متن قرمز هم نشون بدی.
                        }
                    }
                }
                else if(is_on_shop_plant(x_loc, y_loc) > -1)
                {
                    int i = is_on_shop_plant(x_loc,y_loc);
                    plant_type t = plants_shop[i].type;
                    if(t == PLANT_PEA && cd_pea > 0.0f) break;
                    if(t  == PLANT_SUNFLOWER && cd_sunflower > 0.0f)break;
                    if(t == PLANT_ROSE && cd_rose > 0.0f)break;
                    if(t == PLANT_CHOMPER && cd_chomper > 0.0f)break;
                    if(t == PLANT_ICE_PEA && cd_ice_pea > 0.0f) break;

                    selected_plant = plants_shop[i].type;
                }
            }

            hover_col = -1;
            hover_row = -1;
            int mc = -1, mr = -1;
            if(grid_world_to_cell(GetMouseX(), GetMouseY(),&mc, &mr))
            {
                hover_col = mc;
                hover_row = mr;
            }
            bullet_update(bullets,dt);
           
            for(int i = 0;i < MAX_BULLETS; i++)
            {
                if(!bullets[i].active) continue;
                for(int b = 0; b < MAX_ZOMBIES; b++)
                {
                    if(!zombies[b].active)continue;
                    if(bullets[i].row == zombies[b].row && fabsf(bullets[i].x - zombies[b].x) < (TILE_SIZE * 0.05f))
                    {
                        zombies[b].health -= bullets[i].damage;
                        //ice-effect
                        if (bullets[i].type == BULLET_ICE)
                        {
                            zombies[b].slowTimer = 2.0f;   // هر برخورد ریست میشه
                        }
                        bullets[i].active = false;
                        play_sfx(SFX_SPLAT);

                        break;
                    }
                }
            }
            if (!game_over && level1_spawned >= level1_total_zombies)
            {
                bool is_active = false;
                for(int i = 0; i < MAX_ZOMBIES; i++)
                {
                    if(zombies[i].active){is_active = true;break;}
                }
                if(is_active == false)
                {
                    *game = STATE_LEVEL_COMPLETE;
                    game_music_stop();
                    printf("all zombies = %d", level1_spawned);
                    break;
                }
            }
            if(game_over == true)
            {
                *game = STATE_GAME_OVER;
                game_music_stop();
            }
        }break;

        case STATE_LEVEL2:
        {
            if(IsKeyPressed(KEY_C))
            {
                *game = STATE_GAME_OVER;
            }
            game_music_play();
            time_level += dt;
            if(!first_time_level2)
            {
                level2_spawned = 0;
                zombie_spawn_timer = 0.0f;
                wave_timer = 0.0f;
                spawn_interval = 3.0f;

                for(int i = 0; i < GRID_ROWS; i++)
                    mowers[i].available = false;

                first_time_level2 = true;
            }

            //Wave-Update...
            wave_timer += dt;
            if(wave_timer >= 20.0f)
            {
                wave++;
                wave_timer = 0.0f;
                spawn_interval = fmaxf(0.8f, spawn_interval - 0.3f);
            }
            update_suns(dt, &sun_spawn_timer, suns);
            if(level2_total_zombies > level2_spawned)
            {
                zombie_spawn_timer += dt;
                if(zombie_spawn_timer >= spawn_interval)
                {
                    bool has_free = false;
                    for(int i = 0; i < MAX_ZOMBIES; i++)
                    {
                        if(zombies[i].active == false){ has_free = true; break;}
                    }
                    if(has_free)
                    {
                        zombie_spawn_thinking_random_row(zombies, MAX_ZOMBIES);

                        level2_spawned++;
                        printf("Spawn Level2: %d\n", level2_spawned);

                    }
                    zombie_spawn_timer= 0.0f;
                }
            }
            //end_of_wave...
            float rowMul[GRID_ROWS];
            for(int r=0;r<GRID_ROWS;r++) rowMul[r]=1.0f;

            for(int r = 0; r < GRID_ROWS; r++)
            {
                for(int c = 0; c < GRID_COLS; c++)
                {
                    if(plants[r][c].type == PLANT_CHOMPER && plants[r][c].alive)
                    {
                        rowMul[r] = 1.0f/3.0f;
                        break;
                    }
                }
            }

            zombies_update(zombies, MAX_ZOMBIES, dt, plants, &game_over, rowMul,mowers);
            mowers_update(mowers,zombies, MAX_ZOMBIES, dt);
            cd_pea = fmaxf(0.0f, cd_pea - dt);
            cd_sunflower = fmaxf(0.0f, cd_sunflower - dt);
            cd_rose = fmaxf(0.0f, cd_rose - dt);
            cd_chomper = fmaxf(0.0f, cd_chomper - dt);
            cd_ice_pea = fmaxf(0.0f, cd_ice_pea - dt);//new


            apply_rose_effects(plants, dt, rose_heal_acc);


            int evCount = plant_collect_events(plants, dt, plant_events, MAX_PLANT_EVENTS);
            for(int i = 0; i < evCount; i++)
            {
                PlantEvent ev = plant_events[i];
                if(ev.type == EVENT_SPAWN_BULLET)
                {
                    for(int j = 0; j < MAX_ZOMBIES; j++)
                    {
                        int plantX = ORIGIN_X + ev.col * TILE_SIZE;
                        if(zombies[j].row == ev.row && zombies[j].active && zombies[j].x > plantX && zombies[j].x < 1100)
                        {
                            Plant *pp = &plants[ev.row][ev.col];
                            BulletType bt = (pp->type == PLANT_ICE_PEA) ? BULLET_ICE : BULLET_NORMAL;
                            spawn_bullet(ev.row, ev.col, bt, bullets);
                            play_sfx(SFX_THROW);
                            break;
                        }
                    }
                }
                else if(ev.type == EVENT_SPAWN_SUN)
                {
                    spawn_sun_at(suns, ev.row, ev.col);
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                int mc = -1, mr = -1;
                int x_loc = GetMouseX(), y_loc = GetMouseY();
                if(handle_sun_click((Vector2){(float )x_loc,(float )y_loc}, suns, &sun_points))
                {
                    break;
                }
                else if (grid_world_to_cell(x_loc, y_loc, &mc, &mr) && selected_plant != PLANT_NONE)
                {
                    if (grid_can_place_plant(&map, mc, mr))
                    {
                        int cost = PLANT_COST[selected_plant];

                        if (sun_points >= cost)
                        {
                            if (plant_place(plants, selected_plant, mc, mr))
                            {
                                sun_points -= cost;
                                play_sfx(SFX_PLANT);
                                if(selected_plant == PLANT_PEA) cd_pea = 5.0f;
                                else if(selected_plant == PLANT_SUNFLOWER) cd_sunflower = 5.0f;
                                else if(selected_plant == PLANT_ROSE) cd_rose = 5.0f;
                                else if(selected_plant == PLANT_CHOMPER) cd_chomper = 5.0f;
                                else if(selected_plant == PLANT_ICE_PEA) cd_ice_pea = 5.0f;
                                selected_plant = PLANT_NONE;
                            }
                        }
                        else
                        {
                            // این بخش فقط برای دیباگ و تست:
                            printf("Not enough sun for this plant!\n");
                            // اگر خواستی، می‌تونی بعداً کنار موس متن قرمز هم نشون بدی.
                        }
                    }
                }
                else if(is_on_shop_plant(x_loc, y_loc) > -1)
                {
                    int i = is_on_shop_plant(x_loc,y_loc);
                    plant_type t = plants_shop[i].type;
                    if(t == PLANT_PEA && cd_pea > 0.0f) break;
                    if(t  == PLANT_SUNFLOWER && cd_sunflower > 0.0f)break;
                    if(t == PLANT_ROSE && cd_rose > 0.0f)break;
                    if(t == PLANT_CHOMPER && cd_chomper > 0.0f)break;
                    if(t == PLANT_ICE_PEA && cd_ice_pea > 0.0f)break;
                    selected_plant = plants_shop[i].type;
                }
            }


            hover_col = -1;
            hover_row = -1;
            int mc = -1, mr = -1;
            if(grid_world_to_cell(GetMouseX(), GetMouseY(),&mc, &mr))
            {
                hover_col = mc;
                hover_row = mr;
            }
            bullet_update(bullets,dt);

            for(int i = 0;i < MAX_BULLETS; i++)
            {
                if(!bullets[i].active) continue;
                for(int b = 0; b < MAX_ZOMBIES; b++)
                {
                    if(!zombies[b].active)continue;
                    if(bullets[i].row == zombies[b].row && fabsf(bullets[i].x - zombies[b].x) < (TILE_SIZE * 0.05f))
                    {
                        zombies[b].health -= bullets[i].damage;
                        bullets[i].active = false;
                        break;
                    }
                }
            }
            if (!game_over && level2_spawned >= level2_total_zombies)
            {
                bool is_active = false;
                for(int i = 0; i < MAX_ZOMBIES; i++)
                {
                    if(zombies[i].active){is_active = true;break;}
                }
                if(is_active == false)
                {
                    *game = STATE_LEVEL_COMPLETE;
                    game_music_stop();
                    break;
                }
            }
            if(game_over == true)
            {
                *game = STATE_GAME_OVER;
                game_music_stop();
            }
        }break;
        case STATE_LEVEL3:
        {
            if(IsKeyPressed(KEY_C))
            {
                *game = STATE_GAME_OVER;
            }
            game_music_play();
            time_level += dt;
            if(!first_time_level3)
            {
                for(int i = 0; i < GRID_ROWS;i++)
                    mowers[i].available = true;
                sun_points = 1000;
                cd_sunflower = 10000.0f;
                first_time_level3 = true;
            }
            wave_timer += dt;
            if(wave_timer >= 20.0f)
            {
                wave++;
                wave_timer = 0.0f;
                spawn_interval = fmaxf(0.8f, spawn_interval - 0.3f);
            }
            int total_zombies = level3_spawned_thinking + level3_spawned_normal;
            if(total_zombies < level3_total_normal + level3_total_thinking)
            {
                zombie_spawn_timer += dt;
                if(zombie_spawn_timer >= spawn_interval)
                {
                    bool has_free = false;
                    for(int i = 0; i < MAX_ZOMBIES; i++)
                        if(zombies[i].active == false){ has_free = true; break;}
                    if(has_free)
                    {
                        if(level3_spawned_normal < 3)
                        {
                            zombie_spawn_random_row(zombies, MAX_ZOMBIES);
                            level3_spawned_normal++;
                        }
                        else if(level3_spawned_thinking < 2)
                        {
                            zombie_spawn_thinking_random_row(zombies, MAX_ZOMBIES);
                            level3_spawned_thinking++;
                        }
                        else if(level3_spawned_normal < 10)
                        {
                            zombie_spawn_random_row(zombies, MAX_ZOMBIES);
                            level3_spawned_normal++;
                        }
                        else if(level3_spawned_thinking < 13)
                        {
                            zombie_spawn_thinking_random_row(zombies, MAX_ZOMBIES);
                            level3_spawned_thinking++;
                        }
                        else if(level3_spawned_normal < 15)
                        {
                            zombie_spawn_random_row(zombies, MAX_ZOMBIES);
                            level3_spawned_normal++;
                        }
                        else
                        {
                            zombie_spawn_thinking_random_row(zombies, MAX_ZOMBIES);
                            level3_spawned_thinking++;
                        }
                    }
                    zombie_spawn_timer= 0.0f;
                }
            }
            //end_of_wave...
            float rowMul[GRID_ROWS];
            for(int r=0;r<GRID_ROWS;r++) rowMul[r]=1.0f;

            for(int r = 0; r < GRID_ROWS; r++)
            {
                for(int c = 0; c < GRID_COLS; c++)
                {
                    if(plants[r][c].type == PLANT_CHOMPER && plants[r][c].alive)
                    {
                        rowMul[r] = 1.0f/3.0f;
                        break;
                    }
                }
            }

            zombies_update(zombies, MAX_ZOMBIES, dt, plants, &game_over, rowMul, mowers);
            mowers_update(mowers,zombies, MAX_ZOMBIES, dt);
            cd_pea = fmaxf(0.0f, cd_pea - dt);
            cd_rose = fmaxf(0.0f, cd_rose - dt);
            cd_chomper = fmaxf(0.0f, cd_chomper - dt);
            cd_ice_pea = fmaxf(0.0f, cd_ice_pea - dt);//new

            apply_rose_effects(plants, dt, rose_heal_acc);


            int evCount = plant_collect_events(plants, dt, plant_events, MAX_PLANT_EVENTS);
            for(int i = 0; i < evCount; i++)
            {
                PlantEvent ev = plant_events[i];
                if(ev.type == EVENT_SPAWN_BULLET)
                {
                    for(int j = 0; j < MAX_ZOMBIES; j++)
                    {
                        int plantX = ORIGIN_X + ev.col * TILE_SIZE;
                        if(zombies[j].row == ev.row && zombies[j].active && zombies[j].x > plantX && zombies[j].x < 1100)
                        {
                            Plant *pp = &plants[ev.row][ev.col];
                            BulletType bt = (pp->type == PLANT_ICE_PEA) ? BULLET_ICE : BULLET_NORMAL;
                            spawn_bullet(ev.row, ev.col, bt, bullets);
                            play_sfx(SFX_THROW);

                            break;
                        }
                    }
                }
                else if(ev.type == EVENT_SPAWN_SUN)
                {
                    spawn_sun_at(suns, ev.row, ev.col);
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                int mc = -1, mr = -1;
                int x_loc = GetMouseX(), y_loc = GetMouseY();
                if(handle_sun_click((Vector2){(float )x_loc,(float )y_loc}, suns, &sun_points))
                {
                    break;
                }
                else if (grid_world_to_cell(x_loc, y_loc, &mc, &mr) && selected_plant != PLANT_NONE)
                {
                    if (grid_can_place_plant(&map, mc, mr))
                    {
                        int cost = PLANT_COST[selected_plant];

                        if (sun_points >= cost)
                        {
                            if (plant_place(plants, selected_plant, mc, mr))
                            {
                                sun_points -= cost;
                                play_sfx(SFX_PLANT);
                                if(selected_plant == PLANT_PEA) cd_pea = 5.0f;
                                else if(selected_plant == PLANT_SUNFLOWER) cd_sunflower = 5.0f;
                                else if(selected_plant == PLANT_ROSE) cd_rose = 5.0f;
                                else if(selected_plant == PLANT_CHOMPER) cd_chomper = 5.0f;
                                else if(selected_plant == PLANT_ICE_PEA) cd_ice_pea = 5.0f;

                                selected_plant = PLANT_NONE;
                            }
                        }
                        else
                        {
                            // این بخش فقط برای دیباگ و تست:
                            printf("Not enough sun for this plant!\n");
                            // اگر خواستی، می‌تونی بعداً کنار موس متن قرمز هم نشون بدی.
                        }
                    }
                }
                else if(is_on_shop_plant(x_loc, y_loc) > -1)
                {
                    int i = is_on_shop_plant(x_loc,y_loc);
                    plant_type t = plants_shop[i].type;
                    if(t == PLANT_PEA && cd_pea > 0.0f) break;
                    if(t  == PLANT_SUNFLOWER)break;
                    if(t == PLANT_ROSE && cd_rose > 0.0f)break;
                    if(t == PLANT_CHOMPER && cd_chomper > 0.0f)break;
                    if(t == PLANT_ICE_PEA && cd_ice_pea > 0.0f)break;
                    selected_plant = plants_shop[i].type;
                }
            }


            hover_col = -1;
            hover_row = -1;
            int mc = -1, mr = -1;
            if(grid_world_to_cell(GetMouseX(), GetMouseY(),&mc, &mr))
            {
                hover_col = mc;
                hover_row = mr;
            }
            bullet_update(bullets,dt);

            for(int i = 0;i < MAX_BULLETS; i++)
            {
                if(!bullets[i].active) continue;
                for(int b = 0; b < MAX_ZOMBIES; b++)
                {
                    if(!zombies[b].active)continue;
                    if(bullets[i].row == zombies[b].row && fabsf(bullets[i].x - zombies[b].x) < (TILE_SIZE * 0.05f))
                    {
                        zombies[b].health -= bullets[i].damage;
                        bullets[i].active = false;
                        break;
                    }
                }
            }
            if (!game_over && total_zombies >= (level3_total_normal + level3_total_thinking))
            {
                bool is_active = false;
                for(int i = 0; i < MAX_ZOMBIES; i++)
                {
                    if(zombies[i].active){is_active = true;break;}
                }
                if(is_active == false)
                {
                    game_music_stop();
                    *game = STATE_LEVEL_COMPLETE;
                    break;
                }
            }
            if(game_over == true)
            {
                game_music_stop();
                *game = STATE_GAME_OVER;
            }
        }break;
        case STATE_LEVEL4:
        {

            //game_music_play();
            if(!first_time_level4)
            {
                for(int i = 0; i < GRID_ROWS;i++)
                    mowers[i].available = true;
                first_time_level4 = true;
            }
            time_level += dt;
            wave_timer += dt;
            if(wave_timer >= 20.0f)
            {
                wave++;
                wave_timer = 0.0f;
                spawn_interval = fmaxf(0.8f, spawn_interval - 0.3f);
            }
            update_suns(dt, &sun_spawn_timer, suns);
                zombie_spawn_timer += dt;
                if(zombie_spawn_timer >= spawn_interval)
                {
                    bool has_free = false;
                    for(int i = 0; i < MAX_ZOMBIES; i++)
                    {
                        if(zombies[i].active == false){ has_free = true; break;}
                    }
                    if(has_free)
                    {
                        int i = rand();
                        if(i % 3 == 0)
                            zombie_spawn_thinking_random_row(zombies, MAX_ZOMBIES);
                        else
                            zombie_spawn_random_row(zombies, MAX_ZOMBIES);
                    }
                    zombie_spawn_timer= 0.0f;
                }
            float rowMul[GRID_ROWS];
            for(int r=0;r<GRID_ROWS;r++) rowMul[r]=1.0f;
            for(int r = 0; r < GRID_ROWS; r++)
            {
                for(int c = 0; c < GRID_COLS; c++)
                {
                    if(plants[r][c].type == PLANT_CHOMPER && plants[r][c].alive)
                    {
                        rowMul[r] = 1.0f/3.0f;
                        break;
                    }
                }
            }

            zombies_update(zombies, MAX_ZOMBIES, dt, plants, &game_over, rowMul,mowers);
            mowers_update(mowers,zombies, MAX_ZOMBIES, dt);
            cd_pea = fmaxf(0.0f, cd_pea - dt);
            cd_sunflower = fmaxf(0.0f, cd_sunflower - dt);
            cd_rose = fmaxf(0.0f, cd_rose - dt);
            cd_chomper = fmaxf(0.0f, cd_chomper - dt);
            cd_ice_pea = fmaxf(0.0f, cd_ice_pea - dt);//new

            apply_rose_effects(plants, dt, rose_heal_acc);

            int evCount = plant_collect_events(plants, dt, plant_events, MAX_PLANT_EVENTS);
            for(int i = 0; i < evCount; i++)
            {
                PlantEvent ev = plant_events[i];
                if(ev.type == EVENT_SPAWN_BULLET)
                {
                    for(int j = 0; j < MAX_ZOMBIES; j++)
                    {
                        int plantX = ORIGIN_X + ev.col * TILE_SIZE;
                        if(zombies[j].row == ev.row && zombies[j].active && zombies[j].x > plantX && zombies[j].x < 1100)
                        {
                            Plant *pp = &plants[ev.row][ev.col];
                            BulletType bt = (pp->type == PLANT_ICE_PEA) ? BULLET_ICE : BULLET_NORMAL;
                            spawn_bullet(ev.row, ev.col, bt, bullets);
                            play_sfx(SFX_THROW);
                            break;
                        }
                    }
                }
                else if(ev.type == EVENT_SPAWN_SUN)
                {
                    spawn_sun_at(suns, ev.row, ev.col);
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                int mc = -1, mr = -1;
                int x_loc = GetMouseX(), y_loc = GetMouseY();
                if(handle_sun_click((Vector2){(float )x_loc,(float )y_loc}, suns, &sun_points))
                {
                    break;
                }
                else if (grid_world_to_cell(x_loc, y_loc, &mc, &mr) && selected_plant != PLANT_NONE)
                {
                    if (grid_can_place_plant(&map, mc, mr))
                    {
                        int cost = PLANT_COST[selected_plant];

                        if (sun_points >= cost)
                        {
                            if (plant_place(plants, selected_plant, mc, mr))
                            {
                                sun_points -= cost;
                                play_sfx(SFX_PLANT);
                                if(selected_plant == PLANT_PEA) cd_pea = 5.0f;
                                else if(selected_plant == PLANT_SUNFLOWER) cd_sunflower = 5.0f;
                                else if(selected_plant == PLANT_ROSE) cd_rose = 5.0f;
                                else if(selected_plant == PLANT_CHOMPER) cd_chomper = 5.0f;
                                else if(selected_plant == PLANT_ICE_PEA) cd_ice_pea = 5.0f;

                                selected_plant = PLANT_NONE;
                            }
                        }
                        else
                        {
                            // این بخش فقط برای دیباگ و تست:
                            printf("Not enough sun for this plant!\n");
                            // اگر خواستی، می‌تونی بعداً کنار موس متن قرمز هم نشون بدی.
                        }
                    }
                }
                else if(is_on_shop_plant(x_loc, y_loc) > -1)
                {
                    int i = is_on_shop_plant(x_loc,y_loc);
                    plant_type t = plants_shop[i].type;
                    if(t == PLANT_PEA && cd_pea > 0.0f) break;
                    if(t  == PLANT_SUNFLOWER && cd_sunflower > 0.0f)break;
                    if(t == PLANT_ROSE && cd_rose > 0.0f)break;
                    if(t == PLANT_CHOMPER && cd_chomper > 0.0f)break;
                    if(t == PLANT_ICE_PEA && cd_ice_pea > 0.0f)break;
                    selected_plant = plants_shop[i].type;
                }
            }
            hover_col = -1;
            hover_row = -1;
            int mc = -1, mr = -1;
            if(grid_world_to_cell(GetMouseX(), GetMouseY(),&mc, &mr))
            {
                hover_col = mc;
                hover_row = mr;
            }
            bullet_update(bullets,dt);

            for(int i = 0;i < MAX_BULLETS; i++)
            {
                if(!bullets[i].active) continue;
                for(int b = 0; b < MAX_ZOMBIES; b++)
                {
                    if(!zombies[b].active)continue;
                    if(bullets[i].row == zombies[b].row && fabsf(bullets[i].x - zombies[b].x) < (TILE_SIZE * 0.05f))
                    {
                        zombies[b].health -= bullets[i].damage;
                        bullets[i].active = false;
                        break;
                    }
                }
            }
            if(game_over == true || IsKeyPressed(KEY_C))
            {
                game_music_stop();
                int first, second, third;
                FILE *fp = fopen(ASSET("records_survival.txt"), "r");
                if(fp == NULL)
                {
                    error_reading_file = true;
                    *game = STATE_GAME_OVER;
                    break;
                }
                else {
                    fscanf(fp, "%d %d %d", &first, &second, &third);
                    if (first == 0 && third == 0 && second == 0)
                        first = (int)time_level;
                    else if((int)time_level > third)
                    {
                        if((int)time_level > second)
                        {
                            if((int)time_level > first)
                            {
                                third = second;
                                second = first;
                                first = (int)time_level;
                            }
                            else
                            {
                                third = second;
                                second = (int)time_level;
                            }
                        }
                        else
                            third = (int)time_level;
                    }
                }
                best_time = first;
                fclose(fp);
                FILE *fp2 = fopen(ASSET("records_survival.txt"), "w");
                if(fp2 == NULL)
                {
                    error_saving_file = true;
                    *game = STATE_GAME_OVER;
                    break;
                }
                else
                    fprintf(fp2, "%d\n%d\n%d", first, second, third);
                fclose(fp2);
                *game = STATE_GAME_OVER;
            }
        }break;
        case STATE_SHOP:
        {
            if(IsKeyPressed(KEY_ENTER))
                *game = STATE_MENU;
            else
                *game = STATE_SHOP;
        }break;
        case STATE_EXIT:
        {
            CloseWindow();
        }break;
        case STATE_GAME_OVER:
        {
            if(state_changed)
                lose_music_play();
            if(IsKeyPressed(KEY_ENTER))
            {
                lose_music_stop();
                *game = STATE_MENU;
                game_reset();
            }
            if(last_state == STATE_LEVEL4)
            {
                sprintf(str_level4, "YOUR TIME %d\nBEST TIME %d", (int)time_level, best_time);
            }
        }break;
        case STATE_LEVEL_COMPLETE:
        {
            if(state_changed)
                win_music_play();
            if(IsKeyPressed(KEY_ENTER))
            {
                win_music_stop();
                *game = STATE_MENU;
                game_reset();
            }
        }
    }
}
void game_draw(enum game_state* game)
{
    switch (*game)
    {
        case STATE_MENU :
        {
            ClearBackground(BLACK);
            menu_show(background);
        }break;
        case STATE_LEVEL_SELECT:
        {
            ClearBackground(BLACK);
            level_show(level_back);
        }break;
        case STATE_LEVEL1:
        {
            ClearBackground(BLACK);
            if (!level_map_ready)
            {
                grid_init(&map);
                grid_make_checker_grass(&map);
                level_map_ready = true;
                plant_grid_clear(plants);
            }
            grid_draw(&map, grass, soil, asphalt);
            draw_shop_bar(shop_bar, shop_rect);
            draw_plant_shop(plants_use_shop, plants_shop);

            draw_shop_prices(plants_shop, number_of_plants);

            draw_shop_cooldown(plants_shop, number_of_plants, cd_pea, cd_sunflower, cd_rose, cd_chomper,cd_ice_pea, sun_points);

            char sunText[64];
            sprintf(sunText, "Sun: %d", sun_points);
            DrawText(sunText,200,40,30, BLACK);

            char time[60];
            sprintf(time, "Time = %d", (int)time_level);
            DrawText(time, 630, 40, 30, BLACK);

            mouse_on_shop(GetMouseX(),GetMouseY());
            plant_grid_draw(plants);
            draw_suns(suns, sun);
            zombies_draw(zombies, MAX_ZOMBIES);
            draw_bullets(bullets);
            mowers_draw(mowers, mower_tex);

            if(hover_row != -1 && hover_col != -1)
            {
                int x = ORIGIN_X + TILE_SIZE * hover_col;
                int y = ORIGIN_Y + TILE_SIZE * hover_row;
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);
                Color border;
                if(!can_plant) border = RED;
                else if(is_occ) border = ORANGE;
                else border = GREEN;
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE }, 4, border);
            }
            if(hover_row != -1 && hover_col != -1 && selected_plant != PLANT_NONE)
            {
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);

                Color preview = (can_plant && !is_occ) ? Fade(WHITE, 0.45f) : Fade(RED,  .45f);
                plant_draw_preview(plants,selected_plant, hover_col, hover_row, preview);
            }
        }break;
        case STATE_LEVEL2:
        {
            ClearBackground(BLACK);
            if (!level_map_ready)
            {
                grid_init(&map);
                grid_make_checker_grass(&map);
                level_map_ready = true;
                plant_grid_clear(plants);
            }
            grid_draw(&map, grass, soil, asphalt);
            draw_shop_bar(shop_bar, shop_rect);
            draw_plant_shop(plants_use_shop, plants_shop);
            draw_shop_prices(plants_shop, number_of_plants);

            draw_shop_cooldown(plants_shop, number_of_plants, cd_pea, cd_sunflower, cd_rose, cd_chomper, cd_ice_pea, sun_points);


            char sunText[64];
            sprintf(sunText, "Sun: %d", sun_points);
            DrawText(sunText,200,40,30, BLACK);

            char time[60];
            sprintf(time, "Time = %d", (int)time_level);
            DrawText(time, 630, 40, 30, BLACK);

            mouse_on_shop(GetMouseX(),GetMouseY());
            plant_grid_draw(plants);
            draw_suns(suns, sun);
            zombies_draw(zombies, MAX_ZOMBIES);
            draw_bullets(bullets);
            //mowers_draw(mowers, mower_tex);

            if(hover_row != -1 && hover_col != -1)
            {
                int x = ORIGIN_X + TILE_SIZE * hover_col;
                int y = ORIGIN_Y + TILE_SIZE * hover_row;
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);
                Color border;
                if(!can_plant) border = RED;
                else if(is_occ) border = ORANGE;
                else border = GREEN;
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE }, 4, border);
            }
            if(hover_row != -1 && hover_col != -1 && selected_plant != PLANT_NONE)
            {
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);

                Color preview = (can_plant && !is_occ) ? Fade(WHITE, 0.45f) : Fade(RED,  .45f);
                plant_draw_preview(plants,selected_plant, hover_col, hover_row, preview);
            }
        }break;
        case STATE_LEVEL3:
        {
            ClearBackground(BLACK);
            if (!level_map_ready)
            {
                grid_init(&map);
                grid_make_checker_grass(&map);
                level_map_ready = true;
                plant_grid_clear(plants);
            }
            grid_draw(&map, grass, soil, asphalt);
            draw_shop_bar(shop_bar, shop_rect);
            draw_plant_shop(plants_use_shop, plants_shop);
            draw_shop_prices(plants_shop, number_of_plants);

            draw_shop_cooldown(plants_shop, number_of_plants, cd_pea, cd_sunflower, cd_rose, cd_chomper,cd_ice_pea, sun_points);

            char sunText[64];
            sprintf(sunText, "Sun: %d", sun_points);
            DrawText(sunText,200,40,30, BLACK);

            char time[60];
            sprintf(time, "Time = %d", (int)time_level);
            DrawText(time, 630, 40, 30, BLACK);

            mouse_on_shop(GetMouseX(),GetMouseY());
            plant_grid_draw(plants);
            draw_suns(suns, sun);
            zombies_draw(zombies, MAX_ZOMBIES);
            draw_bullets(bullets);
            mowers_draw(mowers, mower_tex);

            if(hover_row != -1 && hover_col != -1)
            {
                int x = ORIGIN_X + TILE_SIZE * hover_col;
                int y = ORIGIN_Y + TILE_SIZE * hover_row;
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);
                Color border;
                if(!can_plant) border = RED;
                else if(is_occ) border = ORANGE;
                else border = GREEN;
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE }, 4, border);
            }
            if(hover_row != -1 && hover_col != -1 && selected_plant != PLANT_NONE)
            {
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);

                Color preview = (can_plant && !is_occ) ? Fade(WHITE, 0.45f) : Fade(RED,  .45f);
                plant_draw_preview(plants,selected_plant, hover_col, hover_row, preview);
            }
        }break;
        case STATE_LEVEL4:
        {
            ClearBackground(BLACK);
            if (!level_map_ready)
            {
                grid_init(&map);
                grid_make_checker_grass(&map);
                level_map_ready = true;
                plant_grid_clear(plants);
            }
            grid_draw(&map, grass, soil, asphalt);
            draw_shop_bar(shop_bar, shop_rect);
            draw_plant_shop(plants_use_shop, plants_shop);
            draw_shop_prices(plants_shop, number_of_plants);
            draw_shop_cooldown(plants_shop, number_of_plants, cd_pea, cd_sunflower, cd_rose, cd_chomper, cd_ice_pea, sun_points);


            char sunText[64];
            sprintf(sunText, "Sun: %d", sun_points);
            DrawText(sunText,200,40,30, BLACK);

            char time[60];
            sprintf(time, "Time = %d", (int)time_level);
            DrawText(time, 630, 40, 30, BLACK);

            mouse_on_shop(GetMouseX(),GetMouseY());
            plant_grid_draw(plants);
            draw_suns(suns, sun);
            zombies_draw(zombies, MAX_ZOMBIES);
            draw_bullets(bullets);
            mowers_draw(mowers, mower_tex);

            if(hover_row != -1 && hover_col != -1)
            {
                int x = ORIGIN_X + TILE_SIZE * hover_col;
                int y = ORIGIN_Y + TILE_SIZE * hover_row;
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);
                Color border;
                if(!can_plant) border = RED;
                else if(is_occ) border = ORANGE;
                else border = GREEN;
                DrawRectangleLinesEx((Rectangle){ (float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE }, 4, border);
            }
            if(hover_row != -1 && hover_col != -1 && selected_plant != PLANT_NONE)
            {
                bool can_plant = grid_can_place_plant(&map, hover_col, hover_row);
                bool is_occ = !plant_cell_is_empty(plants, hover_col, hover_row);

                Color preview = (can_plant && !is_occ) ? Fade(WHITE, 0.45f) : Fade(RED,  .45f);
                plant_draw_preview(plants,selected_plant, hover_col, hover_row, preview);
            }
            if(error_saving_file)
            {
                DrawText("ERROR-SAVING-FILE",370, 480, 20, RAYWHITE);
            }
            if(error_reading_file)
            {
                DrawText("ERROR-SAVING-FILE",370, 480, 20, RAYWHITE);
            }
        }break;
        case STATE_SHOP:
        {
            ClearBackground(BLACK);
            DrawText("NOT-NOW-NEXT-UPDATE", 320, 300, 50, RAYWHITE);
            DrawText("PRESS-ENTER", 440, 350, 40, RAYWHITE);
        }break;
        case STATE_EXIT:
        {
            break;
        }
        case STATE_LEVEL_COMPLETE:
        {
            //ClearBackground(BLACK);
            DrawText("LEVEL COMPLETE!", 380, 340, 50, RAYWHITE);
            DrawText("Press ENTER to continue", 360, 410, 30, RAYWHITE);

        }break;
        case STATE_GAME_OVER:
        {
            //ClearBackground(BLACK);
            DrawText("GAME-OVER",380,340,50, RAYWHITE);
            DrawText("Press ENTER to continue", 360, 410, 30, RAYWHITE);
            if(last_state == STATE_LEVEL4)
            {
                DrawText(str_level4, 380, 450, 30, RAYWHITE);
            }
        }break;
    }
}