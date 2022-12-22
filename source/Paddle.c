#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <citro2d.h>

#include "Paddle.h"

static const int SCREEN_WIDTH = 400;
static const int SCREEN_HEIGHT = 240;

void initPaddle(Paddle* paddle)
{
	paddle->w = 10;
	paddle->h = 60;
	paddle->x = 5;
	paddle->y = SCREEN_HEIGHT/2 - paddle->h/2;
	paddle->score = 0;
	strncpy(paddle->scoreStr, "0", 32);
	paddle->colour = C2D_Color32(255,255,255,255);

	paddle->hit = Mix_LoadWAV("romfs:/sound/sound_hit.wav");
}

void drawPaddle(Paddle* paddle)
{
	C2D_DrawRectSolid(paddle->x, paddle->y, 0, paddle->w, paddle->h, paddle->colour);
}

void paddleCollision(Paddle *paddle)
{
    if(paddle->y < 0)
        paddle->y = 0;

    if(paddle->y + paddle->h > SCREEN_HEIGHT)
        paddle->y = SCREEN_HEIGHT - paddle->h;
}

void addScore(Paddle *paddle, int score)
{
	paddle->score += score;
	sprintf(paddle->scoreStr, "%d", paddle->score);	
}

char *getScoreStr(Paddle *paddle)
{
	return paddle->scoreStr;
}
