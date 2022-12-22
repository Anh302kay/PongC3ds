#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <citro2d.h>

#include "Ball.h"
#include "Paddle.h"

static const int SCREEN_WIDTH = 400;
static const int SCREEN_HEIGHT = 240;

static const float BALL_XVEL_GAIN = 0.765f;
static const float BALL_YVEL_GAIN = 0.765f;

static const float BALL_MAX_XVEL = 15.f;
static const float BALL_MAX_YVEL = 15.f;

void initBall(Ball* ball)
{

    ball->w = 15;
    ball->h = 15;
    ball->x = SCREEN_WIDTH/2 - ball->w/2;
    ball->y = SCREEN_HEIGHT/2 - ball->h/2;

    ball->velX = (rand() % 500 > 250) ? (rand()% 500) / 100.f - 1.f : -(rand() % 500) / 100.f + 1.f;
    ball->velY = (rand() % 500) / 100.f - 1.f;

    ball->colour = C2D_Color32(255, 255, 255, 255);
    ball->collisionTimer = 0;
    ball->startTimer = 0;

    ball->bounce = Mix_LoadWAV("romfs:/sound/sound_bounce.wav");
    ball->score = Mix_LoadWAV("romfs:/sound/sound_score.wav");
}

void resetBall(Ball *ball)
{
    ball->startTimer = osGetTime() + 2000;
    ball->x = SCREEN_WIDTH / 2 - ball->w / 2;
    ball->y = SCREEN_HEIGHT / 2 - ball->h / 2;
    ball->velX = (rand() % 500 > 250) ? (rand() % 500) / 100.f - 1.f : -(rand() % 500) / 100.f + 1.f;
    ball->velY = (rand() % 500) / 100.f - 1.f;
}

void drawBall(Ball* ball)
{
    C2D_DrawRectSolid(ball->x, ball->y, 0, ball->w, ball->h, ball->colour);
}

void updateBall(Ball* ball)
{
    if(ball->startTimer < osGetTime())
    {
        ball->x += ball->velX;
        ball->y += ball->velY;
    }
}
void ballCollision(Ball* ball, Paddle* player, Paddle* AI)
{
    //bounce
    if(ball->y < 0)
    {
        ball->y = 0;
        ball->velY -= 0.02f;
        ball->velY = -ball->velY;
        ball->velX = (ball->velX > 0) ? (ball->velX) + 0.02f : (ball->velX) - 0.02f;
        Mix_PlayChannel(-1, ball->bounce, 0);

    }
    else if (ball->y > SCREEN_HEIGHT - ball->h)
    {
        ball->y = SCREEN_HEIGHT - ball->h;
        ball->velY += 0.02f;
        ball->velY = -ball->velY;
        ball->velX = (ball->velX > 0) ? (ball->velX) + 0.02f : (ball->velX) - 0.02f;
        Mix_PlayChannel(-1, ball->bounce, 0);

    }

    if (player->x + player->w > ball->x && player->x < ball->x + ball->h && player->y <= ball->y + ball->h && player->y + player->h >= ball->y && ball->collisionTimer < osGetTime())
    {
        ball->x = player->x + player->w;
        ball->collisionTimer = osGetTime() + 100;
        if ((ball->y - player->y) / player->h < 0.33f)
        {
            ball->velX = -(ball->velX) + BALL_XVEL_GAIN;
            ball->velY = (ball->velY > 0) ? -(ball->velY) - BALL_YVEL_GAIN : (ball->velY) - BALL_YVEL_GAIN;
        }
        else if ((ball->y - player->y) / player->h < 0.66f)
        {
            ball->velX = -(ball->velX) + BALL_XVEL_GAIN;
            ball->velY = (rand() % 1000) / 500.f - 1.f;
        }
        else if ((ball->y - player->y) / player->h < 1.f)
        {
            ball->velX = -(ball->velX) + BALL_XVEL_GAIN;
            ball->velY = (ball->velY > 0) ? (ball->velY) + BALL_YVEL_GAIN : -(ball->velY) + BALL_YVEL_GAIN;
        }
        Mix_PlayChannel(-1, player->hit, 0);
    }

    if (ball->x + ball->w > AI->x)
    {
        ball->collisionTimer = osGetTime() + 100;
        ball->velX = -ball->velX - BALL_XVEL_GAIN;
        ball->velY = (ball->velY > 0) ? -(ball->velY) - BALL_YVEL_GAIN : -(ball->velY) + BALL_YVEL_GAIN;
        Mix_PlayChannel(-1, AI->hit, 0);
    }

    if(ball->x + ball->w < -100)
    {
        addScore(AI, 1);
        player->y = SCREEN_HEIGHT / 2 - player->h / 2;
        AI->y = SCREEN_HEIGHT / 2 - AI->h / 2;
        resetBall(ball);
        Mix_PlayChannel(-1, ball->score, 0);
    }

    if(ball->velX > BALL_MAX_XVEL)
        ball->velX = BALL_MAX_XVEL;

    if (ball->velY > BALL_MAX_YVEL)
        ball->velY = BALL_MAX_YVEL;
}
