#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <citro2d.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

typedef struct
{
    int w, h;
    int x, y;
    int score;
    char scoreStr[32];
    u32 colour;
    Mix_Chunk* hit;
} Paddle;

void initPaddle(Paddle *paddle);

void drawPaddle(Paddle *paddle);

void paddleCollision(Paddle* paddle);

void addScore(Paddle* paddle, int score);

char* getScoreStr(Paddle* paddle);