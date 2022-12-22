// Simple citro2d sprite drawing example
// Images borrowed from:
//   https://kenney.nl/assets/space-shooter-redux

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <citro2d.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "Paddle.h"
#include "Ball.h"

static const int SCREEN_WIDTH  = 400; 
static const int SCREEN_HEIGHT = 240;

const int PADDLE_VELOCITY = 3;

bool shouldExit = false;
bool onTitle = true;
C2D_Font font;
C2D_TextBuf textBuf;
Paddle player;
Paddle AI;
Ball ball;

void pollInput(u32* kDown, u32* kHeld)
{
	if (*kDown & KEY_START)
		shouldExit = true; // break in order to return to hbmenu

	if (*kDown != 0 && *kDown != KEY_UP && *kDown != KEY_DOWN && *kDown != KEY_LEFT && *kDown != KEY_RIGHT && onTitle == true)
		onTitle = false;

	if(*kHeld & KEY_UP)
		player.y -= PADDLE_VELOCITY;

	if (*kHeld & KEY_DOWN)
		player.y += PADDLE_VELOCITY;
}

void drawText(const char* textStr, float posX, float posY, float scaleX, float scaleY, u32 colour)
{
	C2D_TextBufClear(textBuf);
	C2D_Text text;
	C2D_TextFontParse(&text, font, textBuf, textStr);
	C2D_TextOptimize(&text);
	C2D_DrawText(&text, C2D_AlignCenter | C2D_WithColor, posX, posY, 0, scaleX, scaleY, colour);
}

void initObjects()
{
	initPaddle(&player);
	initPaddle(&AI);
	AI.x = SCREEN_WIDTH - 5 - AI.w;
	initBall(&ball);
}

void deinitObjects()
{
	Mix_FreeChunk(player.hit);
	Mix_FreeChunk(AI.hit);
	Mix_FreeChunk(ball.bounce);
	Mix_FreeChunk(ball.score);
}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	srand(osGetTime());
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	//consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	font = C2D_FontLoad("romfs:/gfx/GravityBold8.bcfnt");
	textBuf = C2D_TextBufNew(4096);

	initObjects();

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

		pollInput(&kDown, &kHeld);

		AI.y = ball.y - AI.h / 2;
		paddleCollision(&player);
		paddleCollision(&AI);
		if(onTitle)
		{
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 1.0f, 1.0f));
			C2D_SceneBegin(top);

			drawPaddle(&player);
			drawPaddle(&AI);
			drawBall(&ball);

			drawText("PONG", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8, 2.f, 2.f, C2D_Color32(255, 255, 255, 255));
			drawText("PRESS ANY KEY TO START", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 32, 0.8f, 0.8f, C2D_Color32(255, 255, 255, 255));

			C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 1.0f, 1.0f));
			C2D_SceneBegin(bottom);

			drawText("PRESS START TO QUIT", 320/2, 240/2, 0.8f, 0.8f, C2D_Color32(255,255,255,255));

			C3D_FrameEnd(0);
		}
		else
		{
			updateBall(&ball);
			ballCollision(&ball, &player, &AI);
			

			if(shouldExit)
				break;

			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32(0, 0, 255, 255));
			C2D_SceneBegin(top);

			C2D_DrawLine(SCREEN_WIDTH / 2 - 1, SCREEN_HEIGHT / 10, C2D_Color32(255, 255, 255, 255), SCREEN_WIDTH / 2 - 1, SCREEN_HEIGHT - SCREEN_HEIGHT / 10, C2D_Color32(255, 255, 255, 255), 3, 0);

			drawPaddle(&player);
			drawPaddle(&AI);
			drawBall(&ball);

			drawText(player.scoreStr, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 12, 1.f, 1.f, C2D_Color32(255, 255, 255, 255));
			drawText(AI.scoreStr, SCREEN_WIDTH - SCREEN_WIDTH / 4, SCREEN_HEIGHT / 12, 1.f, 1.f, C2D_Color32(255, 255, 255, 255));

			C3D_FrameEnd(0);
		}

	}

	deinitObjects();
	C2D_TextBufDelete(textBuf);
	C2D_FontFree(font);

	Mix_Quit();
	SDL_Quit();

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
