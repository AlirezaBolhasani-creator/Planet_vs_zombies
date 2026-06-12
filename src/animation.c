#include "animation.h"
// لود sprite sheet و تنظیم پارامترهای اولیه
Animation Animation_Load(const char* file,
                         int frameWidth,
                         int frameHeight,
                         int frameCount,
                         float fps,
                         bool loop,
                         int padding)
{
    Animation a = (Animation){0};

    Image img = LoadImage(file);
    if (!img.data) return a;

    // حذف رنگ بنفش پس‌زمینه
    ImageColorReplace(&img, (Color){128,0,128,255}, (Color){0,0,0,0});

    a.texture = LoadTextureFromImage(img);
    UnloadImage(img);

    a.frameWidth  = frameWidth;
    a.frameHeight = frameHeight;
    a.frameCount  = frameCount;
    a.fps         = fps;
    a.loop        = loop;
    a.padding     = padding;

    return a;
}

// ریست انیمیشن
void Animation_Reset(Animation* anim)
{
    anim->currentFrame = 0;
    anim->timer = 0.0f;
    anim->finished = false;
}

// آپدیت فریم بر اساس dt و fps
void Animation_Update(Animation* anim, float dt)
{
    if (!anim || anim->finished || anim->fps <= 0) return;

    anim->timer += dt;
    float frameTime = 1.0f / anim->fps;

    while (anim->timer >= frameTime)
    {
        anim->timer -= frameTime;
        anim->currentFrame++;

        if (anim->currentFrame >= anim->frameCount)
        {
            if (anim->loop) anim->currentFrame = 0;
            else {
                anim->currentFrame = anim->frameCount - 1;
                anim->finished = true;
            }
        }
    }
}

// رسم فریم فعلی از sprite sheet
void Animation_Draw(const Animation* anim, Vector2 pos, float scale)
{
    if (!anim || anim->texture.id == 0) return;

    float srcX = anim->currentFrame * (anim->frameWidth + anim->padding);

    Rectangle src = { srcX, 0,
                      (float)anim->frameWidth,
                      (float)anim->frameHeight };

    Rectangle dst = { pos.x, pos.y,
                      anim->frameWidth * scale,
                      anim->frameHeight * scale };

    DrawTexturePro(anim->texture, src, dst, (Vector2){0,0}, 0.0f, WHITE);
}
void Animation_Unload(Animation* anim)
{
    if (!anim) return;
    if (anim->texture.id != 0) UnloadTexture(anim->texture);
    anim->texture = (Texture2D){0};
}
