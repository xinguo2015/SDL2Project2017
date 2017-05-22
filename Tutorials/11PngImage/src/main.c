#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "utilities.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

int            gGameover = 0;
char           gMediaPath[256] = "../Media";
SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
SDL_Color      gBackgroundColor = { 128, 128, 128, 255 };

int gShowHeart = 1;

static void cleanAll()
{
	if( gMainRenderer ) SDL_DestroyRenderer(gMainRenderer), gMainRenderer = NULL;
	if( gMainWindow ) SDL_DestroyWindow(gMainWindow), gMainWindow = NULL;
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
	if( ! (gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED)) ) {
		printf("Error create SDL Renderer\n");
		cleanAll();	exit(-1);
	}
	// 初始化 SDL Image 库
	// IMG_INIT_JPG
	// IMG_INIT_PNG
	// IMG_INIT_TIF
	if( IMG_Init(IMG_INIT_PNG)<0 )
	{
		printf("Couldn't initialize SDL Image: %s\n",IMG_GetError());
		cleanAll();	exit(-1);
	}
}

void endApp()
{
	IMG_Quit();
	SDL_DestroyRenderer(gMainRenderer);
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
}


void display()
{
	// background image
	SDL_SetRenderDrawColor(gMainRenderer, gBackgroundColor.r, 
		gBackgroundColor.g, gBackgroundColor.b, gBackgroundColor.a);
	SDL_RenderClear(gMainRenderer);
	SDL_Texture * texture = IMG_LoadTexture(gMainRenderer, FullPath("/default/background.png"));
	SDL_RenderCopy(gMainRenderer, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
	if( gShowHeart>=1 ) {
		// a big heart
		texture = IMG_LoadTexture(gMainRenderer, FullPath("/default/heart.png"));
		SDL_RenderCopy(gMainRenderer, texture, NULL, NULL);
		if( gShowHeart>=2 ) { 
			// small heart
			SDL_Rect r2 = { gMainWinRect.w/4, gMainWinRect.h/4, gMainWinRect.w/2, gMainWinRect.h/2 };
			SDL_RenderCopy(gMainRenderer, texture, NULL, &r2);
		}
		SDL_DestroyTexture(texture);
	}
	// present the result
	SDL_RenderPresent(gMainRenderer);
}

void handleEvent(SDL_Event* e)
{
	switch (e->type) {
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		gShowHeart = (gShowHeart+1)%3;
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_KEYDOWN:
		break;
	case SDL_KEYUP:
		break;
	default: 
		break;
	}
}

void runMainLoop()
{
	SDL_Event e; 
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
			handleEvent( &e );
		}
		// 做一些其他的事情。。。。。。。。
		display(); // 绘制画面
		SDL_Delay(10); // 延时10ms，避免独霸CPU
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
	runMainLoop(); // main loop 
	endApp();
	return 0;
}

