#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <citro2d.h>



#include "Paddle.h"

typedef struct
{
    int w, h;
    float x, y;
    float velX, velY;
    u32 colour;
    u64 collisionTimer;
    u64 startTimer;
    Mix_Chunk* bounce;
    Mix_Chunk* score;
} Ball;

void initBall(Ball* ball);

void resetBall(Ball* ball);

void drawBall(Ball* ball);

void updateBall(Ball* ball);

void ballCollision(Ball* ball, Paddle* player, Paddle* AI);
