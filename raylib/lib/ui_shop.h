#ifndef UI_SHOP_H
#define UI_SHOP_H
#include "raylib.h"
#include "plant.h"
#include <stdbool.h>
#include "config.h"
void draw_shop_bar(Texture2D shop, Texture2D shop_rect);
void draw_plant_shop(Texture2D plants_tex[number_of_plants], Plant plants[number_of_plants]);
void set_plants_shop(Plant plants[number_of_plants], plant_type type, int i);
int is_on_shop_plant(int x, int y);
void mouse_on_shop(int x, int y);
Rectangle shop_get_slot_rect(int i);


#endif //UI_SHOP_H