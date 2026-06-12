#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Texture2D texture;

    int frameWidth;
    int frameHeight;
    int frameCount;

    float fps;
    bool loop;

    int currentFrame;
    float timer;
    bool finished;

    int padding; // فاصله بین فریم‌ها (مثلاً 1)
} Animation;

// Loads sprite sheet animation
Animation Animation_Load(
        const char* file,
        int frameWidth,
        int frameHeight,
        int frameCount,
        float fps,
        bool loop,
        int padding
);

void Animation_Reset(Animation* anim);
void Animation_Update(Animation* anim, float dt);
void Animation_Draw(const Animation* anim, Vector2 pos, float scale);
void Animation_Unload(Animation* anim);

#endif
