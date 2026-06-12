#include "ui_shop.h"
// موقعیت و اندازه‌های UI شاپ (ثابت برای همه‌ی slot ها)
#define y_start 200
#define x_start 150
#define SLOT_W 100
#define GAP 150

void draw_shop_bar(Texture2D shop, Texture2D shop_rect)
{
    // 1) بک‌گراند نوار شاپ بالا
    Rectangle src_bar = {0, 0, (float)shop.width, (float)shop.height};
    Rectangle dst_bar = {0, 0, 1200, 250};
    DrawTexturePro(shop, src_bar, dst_bar, (Vector2){0,0}, 0, WHITE);

    // 2) قاب/اسلات‌های خالی (جای کارت‌ها)
    for(int i = 0; i < number_of_plants; i++)
    {
        Rectangle src_rect = {0, 0, shop_rect.width, shop_rect.height};
        Rectangle dst_rect = {x_start * i + x_start, y_start , 100, 100};
        Vector2 origin_rect = {dst_rect.width/2, dst_rect.height/2};
        DrawTexturePro(shop_rect, src_rect, dst_rect, origin_rect,0, WHITE);
    }
}

void set_plants_shop(Plant plants[number_of_plants], plant_type type, int i)
{
    // لیست گیاه‌های قابل انتخاب در شاپ (فقط نوع)
    plants[i].type = type;
}

void draw_plant_shop(Texture2D plants_tex[number_of_plants], Plant plants[number_of_plants])
{
    // رسم عکس گیاه‌ها روی اسلات‌ها
    for(int i = 0; i < number_of_plants; i++)
    {
        Rectangle src_rect = {0, 0, plants_tex[i].width, plants_tex[i].height};
        Rectangle dst_rect = {x_start * i + x_start - 55 , y_start - 50, 100, 100};
        DrawTexturePro(plants_tex[i], src_rect, dst_rect, (Vector2){0,0}, 0, WHITE);
    }
}

int is_on_shop_plant(int x, int y)
{
    // تشخیص اینکه موس روی کدوم اسلات شاپه (برای کلیک انتخاب)
    int top = y_start - 50;
    int bottom = y_start + SLOT_W - 50;
    if (y < top || y > bottom) return -1;

    for (int i = 0; i < number_of_plants; i++)
    {
        int centerX = x_start + i * GAP;
        int left = centerX - SLOT_W/2;
        int right = centerX + SLOT_W/2;

        if (x >= left && x <= right) return i;
    }
    return -1;
}
void mouse_on_shop(int x, int y)
{
    // هایلایت کردن اسلات وقتی موس روشه (feedback UI)
    int top = y_start - 50;
    int bottom = y_start + SLOT_W - 50;
    if (y < top || y > bottom) return;

    for (int i = 0; i < number_of_plants; i++)
    {
        int centerX = x_start + i * GAP;
        int left = centerX - SLOT_W/2;
        int right = centerX + SLOT_W/2;

        if (x >= left && x <= right)
        {
            Rectangle lines = {left, top, SLOT_W, SLOT_W};
            DrawRectangleLinesEx(lines, 5, YELLOW);
        }
    }
}
Rectangle shop_get_slot_rect(int i)
{
    // یک تابع کمکی: مختصات دقیق اسلات iام برای رسم کولداون/قیمت
    return (Rectangle){ x_start * i + x_start - 50, y_start - 50, 100, 100 };
}

void draw_shop_cooldown(Plant shopPlants[], int number_of_Plants,
                        float cd_pea,float cd_sunflower, float cd_rose, float cd_chomper,float cd_ice_pea,
                        int sun_points)
{
    // روکش (overlay) برای کولداون و کمبود خورشید
    const float cd_total = 5.0f;

    for (int i = 0; i < number_of_Plants; i++)
    {
        plant_type t = shopPlants[i].type;

        // کولداون هر گیاه از game.c میاد
        float cd = 0.0f;
        if (t == PLANT_PEA) cd = cd_pea;
        else if (t == PLANT_SUNFLOWER) cd = cd_sunflower;
        else if (t == PLANT_ROSE) cd = cd_rose;
        else if (t == PLANT_CHOMPER) cd = cd_chomper;
        else if(t == PLANT_ICE_PEA) cd = cd_ice_pea;
        bool not_enough_sun = (sun_points < PLANT_COST[t]);
        Rectangle r = shop_get_slot_rect(i);

        // نسبت پر شدن کاور
        float ratio = cd / cd_total;
        if (ratio < 0.0f) ratio = 0.0f;
        if (ratio > 1.0f) ratio = 1.0f;

        if (not_enough_sun)
        {
            // اگر خورشید کافی نیست: قرمز نیمه‌شفاف
            DrawRectangleRec(r, Fade(RED, 0.5f));
        }
        else if (cd > 0.0f)
        {
            // اگر در کولداونه: خاکستری از بالا پر میشه
            Rectangle cover = { r.x, r.y, r.width, r.height * ratio };
            DrawRectangleRec(cover, Fade(GRAY, 0.65f));
        }
    }
}

void draw_shop_prices(Plant shopPlants[], int count)
{
    // نمایش قیمت هر گیاه داخل اسلات (با PLANT_COST)
    for (int i = 0; i < count; i++)
    {
        plant_type t = shopPlants[i].type;
        int cost = PLANT_COST[t];

        Rectangle r = shop_get_slot_rect(i);

        char txt[16];
        sprintf(txt, "%d", cost);

        // پایین وسط اسلات
        int tx = (int)(r.x + r.width - 60);
        int ty = (int)(r.y + r.height - 20);

        DrawText(txt, tx, ty, 20, BLACK);
    }
}
