#ifndef SHOP_H
#define SHOP_H
#include "plant.h"
#include "ui_shop.h"
typedef struct
{
    plant_type type;
    int value;
    bool is_bought;
}plant_shop;
bool write_in_file(plant_type* type);
bool read_in_file(plant_type * type);
void shop_show(Texture2D shop_back);
void draw_all_cards(Texture2D* all_plants, bool *is_bought, Texture2D forbidden);
bool can_buy_plant(int price, int account);
bool is_mouse_on_shop(int x, int y);
void draw_lines_orange(int x, int y);
#endif //SHOP_H