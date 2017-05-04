#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "utilities.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

#define TRUE  1
#define FALSE 0

typedef int BOOL;

typedef struct {
	int mWidth, mHeight;
	int mMouseFocus;
	int mKeyboardFocus;
	BOOL mMinimized;
	BOOL mFullScreen;
}
WinState;

WinState gWinState;

int            gGameover = 0;
char           gMediaPath[256] = "";
SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
SDL_Color      gBackgroundColor = { 128, 128, 128, 255 };


static void cleanAll()
{
	if( gMainRenderer ) 
		SDL_DestroyRenderer(gMainRenderer), gMainRenderer = NULL;
	if( gMainWindow ) 
		SDL_DestroyWindow(gMainWindow), gMainWindow = NULL;
}

void initApp(int argc, char *argv[])
{
	SDL_SetHint(SDL_HINT_IME_INTERNAL_EDITING, "1" );
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS );
	// 创建窗口
	if( ! (gMainWindow = SDL_CreateWindow(argv[0], gMainWinRect.x, gMainWinRect.y,
		gMainWinRect.w, gMainWinRect.h, 0) ) ) {
			printf("Error create SDL window\n");
			cleanAll();	exit(-1);
	}
	// 创建绘制环境
	if( ! (gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, 
		SDL_RENDERER_ACCELERATED)) ) {
			printf("Error create SDL Renderer\n");
			cleanAll();	exit(-1);
	}
	// 初始化 true type font使用
	if ( TTF_Init() < 0 ) {
		printf("Couldn't initialize TTF: %s\n",SDL_GetError());
		cleanAll();	exit(-1);
	}
}

void endApp()
{
	TTF_Quit();
	SDL_DestroyRenderer(gMainRenderer);
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
}


void display()
{
	char msg[256];
	int x = 30, y = 50;
	SDL_SetRenderDrawColor(gMainRenderer, gBackgroundColor.r, 
		gBackgroundColor.g, gBackgroundColor.b, gBackgroundColor.a);
	SDL_RenderClear(gMainRenderer);
	// draw something
	sprintf(msg, "Mouse focus = %s", (gWinState.mMouseFocus) ? "ON" : "OFF");
	drawstring(msg, x, y, FullPath("/default/FreeSerif.ttf"), 30, Int2Color(0xFFFFFF00));

	sprintf(msg, "Keyboard focus = %s", (gWinState.mKeyboardFocus) ? "ON" : "OFF");
	drawstring(msg, x, y+=40, FullPath("/default/FreeSerif.ttf"), 30, Int2Color(0xFFFF00FF));

	drawstring(gWinState.mFullScreen ? "Full Screen" : "Not Full Screen",
		x, y+=40, FullPath("/default/FreeSerif.ttf"), 30, Int2Color(0xFF00FFFF));

	// present the result
	SDL_RenderPresent(gMainRenderer);
}

void handleEvent(SDL_Event* e)
{
	//Window event occured
	if (e->type == SDL_WINDOWEVENT)
	{
		switch (e->window.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			//Get new dimensions and repaint on window size change
			gWinState.mWidth = e->window.data1;
			gWinState.mHeight = e->window.data2;
			break;

		case SDL_WINDOWEVENT_EXPOSED:
			//Repaint on exposure
			break;

		case SDL_WINDOWEVENT_ENTER:
			//Mouse entered window
			gWinState.mMouseFocus = TRUE;
			break;

		case SDL_WINDOWEVENT_LEAVE:
			//Mouse left window
			gWinState.mMouseFocus = FALSE;
			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			//Window has keyboard focus
			gWinState.mKeyboardFocus = TRUE;
			break;

		case SDL_WINDOWEVENT_FOCUS_LOST:
			//Window lost keyboard focus
			gWinState.mKeyboardFocus = FALSE;
			break;

		case SDL_WINDOWEVENT_MINIMIZED:
			//Window minimized
			gWinState.mMinimized = TRUE;
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			//Window maxized
			gWinState.mMinimized = FALSE;
			break;

		case SDL_WINDOWEVENT_RESTORED:
			//Window restored
			gWinState.mMinimized = FALSE;
			break;
		}
	}
	else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN)
	{
		SDL_DestroyRenderer(gMainRenderer);
		// toggle full screen
		if (gWinState.mFullScreen)
		{
			SDL_SetWindowFullscreen(gMainWindow, SDL_FALSE);
			gWinState.mFullScreen = FALSE;
		}
		else
		{
			SDL_SetWindowFullscreen(gMainWindow, SDL_TRUE);
			gWinState.mFullScreen = TRUE;
		}
		if( ! (gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED)) ) {
			printf("Error create SDL Renderer\n");
			cleanAll();	exit(-1);
		}
	}
}

void runMainLoop()
{
	SDL_Event e; // 处理事件
	while ( !gGameover ) 
	{
		while ( !gGameover && SDL_PollEvent(&e)) 
		{
			if((e.type == SDL_KEYUP && e.key.keysym.sym==SDLK_ESCAPE) ||
				e.type == SDL_QUIT) //user close window or press ESC key
			{
				gGameover = 1; // 终止应用程序
			}
			// other events ...
			handleEvent(&e);
		}
		// do the main job
		display();
		// 延时10ms，避免独霸CPU
		SDL_Delay(10); 
	}
}

int main(int argc, char *argv[]) 
{
	if( argc>1 ) 
		strcpy(gMediaPath, argv[1]);
	else {
		strcpy(gMediaPath, SDL_GetBasePath());
		strcat(gMediaPath, "../../../Media");
	}
	printf("base path = %s\n", SDL_GetBasePath());
	printf("media path = %s\n", gMediaPath);

	initApp(argc,argv);
	runMainLoop(); // Enter main loop 
	endApp();

	return 0;
}

