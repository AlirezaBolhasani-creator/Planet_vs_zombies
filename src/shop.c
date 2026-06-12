#include "plant.h"
#include "ui_shop.h"
#include "string.h"
#include <stdio.h>
#include "game.h"
#include "shop.h"

bool read_in_file(plant_type* type)
{
    FILE *fp = fopen(ASSET("selected_plants.txt"), "r");
    if (fp == NULL)
        return false;
    for (int i = 0; i < number_of_plants; i++) {
        char st[100];
        fscanf(fp, " %s", st);
        if (strcmp(st, "PEA") == 0) type[i] = PLANT_PEA;
        else if (strcmp(st, "ROSE") == 0) type[i] = PLANT_ROSE;
        else if (strcmp(st, "SUNFLOWER") == 0) type[i] = PLANT_SUNFLOWER;
        else if(strcmp(st, "CHOMPER") == 0) type[i] = PLANT_CHOMPER;
        // add more type later...
    }
    fclose(fp);
    return true;
}
bool write_in_file(plant_type* type)
{
    FILE *fp = fopen(ASSET("selected_plants.txt"), "r+");
    if(fp == NULL)
        return false;
    for(int i = 0; i < number_of_plants; i++)
    {
        if(type[i] == PLANT_PEA)
            fprintf(fp,"%s\n", "PEA");
        else if(type[i] == PLANT_ROSE)
            fprintf(fp, "%s\n", "WALNUT");
        else if(type[i] == PLANT_SUNFLOWER)
            fprintf(fp, "%s\n", "SUNFLOWER");
        //add more type later...
    }
    fclose(fp);
    return true;
}
void shop_show(Texture2D shop_back)
{
    float screenW = (float)GetScreenWidth();
    float screenH = (float)GetScreenHeight();
    float imageW  = (float)shop_back.width;
    float imageH  = (float)shop_back.height;

    float scale = (screenW / imageW > screenH / imageH) ? screenW / imageW : screenH / imageH;

    float finalW = imageW * scale;
    float finalH = imageH * scale;

    float offsetX = (screenW - finalW) / 2;
    float offsetY = (screenH - finalH) / 2;
    Rectangle src = {0,0, imageW, imageH};
    Rectangle dst  = {offsetX, offsetY,finalW, finalH};
    Vector2 origin = {0,0};
    DrawTexturePro(shop_back,src, dst,origin, 0.0f, WHITE);
}
void draw_all_cards(Texture2D* all_plants , bool* is_bought, Texture2D forbidden)
{
    //line 1:
    for(int i = 0; i < all_plants_num; i++)
    {
        Rectangle src = {0,0, all_plants[i].width, all_plants[i].height};
        Rectangle dst = {260*i + 250,200, 200, 200};
        Vector2 origin = {0,0};
        DrawRectangle(dst.x, dst.y, dst.width, dst.height,  (Color){255, 255, 255, 230});
        DrawRectangleLinesEx(dst, 5,RED);
        DrawTexturePro(all_plants[i],src, dst, origin, 0, WHITE);
        //each price change later
        DrawText("200",260*i + 300,400,50,RED);
        if(!is_bought[i])
        {
            Rectangle src_forbidden = {0,0 , forbidden.width, forbidden.height};
            Rectangle dst_forbidden = {250 * i + 360 , 300, 250,250};
            Vector2 origin_forbidden = {dst_forbidden.width/2, dst_forbidden.height/2};
            if (i == 0) dst_forbidden.x -= 10;
            if (i == 2) dst_forbidden.x += 10;
            DrawTexturePro(forbidden, src_forbidden, dst_forbidden ,origin_forbidden,0,WHITE);
        }
    }
    //line 2: later change i and tereshold...
    for(int i = 0; i < all_plants_num; i++)
    {
        Rectangle src = {0,0, all_plants[i].width, all_plants[i].height};
        Rectangle dst = {260*i + 250,500, 200, 200};
        Vector2 origin = {0,0};
        DrawRectangle(dst.x, dst.y, dst.width, dst.height,  (Color){255, 255, 255, 200});
        DrawRectangleLinesEx(dst, 5,RED);
        DrawTexturePro(all_plants[i],src, dst, origin, 0, WHITE);
        //each price change later
        DrawText("200",260*i + 300,700,50,RED);
        if(!is_bought[i])
        {
            Rectangle src_forbidden = {0,0 , forbidden.width, forbidden.height};
            Rectangle dst_forbidden = {250 * i + 360 , 300, 250,250};
            Vector2 origin_forbidden = {dst_forbidden.width/2, dst_forbidden.height/2};
            if (i == 0) dst_forbidden.x -= 10;
            if (i == 2) dst_forbidden.x += 10;
            DrawTexturePro(forbidden, src_forbidden, dst_forbidden ,origin_forbidden,0,WHITE);
        }
    }
}
bool can_buy_plant(int price, int account)
{
    return account >= price;
}
bool is_mouse_on_shop(int x, int y)
{
    if(x < 250 || y < 200 || y > 700 || x > 1230)
        return false;
    else if(y > 200 && y < 400)
    {
        for(int i = 0; i < 3; i++)
        {
            int left = 260 * i + 250;
            int right = left + 200;
            if(x < right && x > left)
                return true;
        }
    }
    else if(y < 700 && y > 500)
    {
        for(int i = 0; i < 3; i++)
        {
            int left = 260 * i + 250;
            int right = left + 200;
            if(x < right && x > left)
                return true;
        }
    }
    return false;
}