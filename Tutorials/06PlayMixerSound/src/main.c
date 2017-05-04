#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "utilities.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

#define BOOL  int
#define FALSE 0
#define TRUE  1

int            gGameover = 0;
char           gMediaPath[256] = "";
SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
SDL_Color      gBackgroundColor = { 0, 0, 0, 255 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;

typedef struct 
{
	int mousex;     // x - mouse position
	int mousey;     // y - mouse position
	int mousedown;  // 1 - yes, 0 - no

	int keypressed; // key that was pressed
	int keymod;     // key modifier flags (such as shift pressed)
	int keychar;    // char that is input

	char * music;
}
UIState;

UIState uistate;

//The music that will be played
Mix_Music *gMusic = NULL;
//The sound effects that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

static void cleanAll()
{
	if( gMainRenderer ) SDL_DestroyRenderer(gMainRenderer), gMainRenderer = NULL;
	if( gMainWindow ) SDL_DestroyWindow(gMainWindow), gMainWindow = NULL;
}

BOOL loadMedia()
{
	//Load music
	gMusic = Mix_LoadMUS(FullPath("/default/wave/beat.wav"));
	if (gMusic == NULL)	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		return FALSE;
	}

	//Load sound effects
	gScratch = Mix_LoadWAV(FullPath("/default/wave/scratch.wav"));
	if (gScratch == NULL)	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return FALSE;
	}

	gHigh = Mix_LoadWAV(FullPath("/default/wave/high.wav"));
	if (gHigh == NULL) {
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return FALSE;
	}

	gMedium = Mix_LoadWAV(FullPath("/default/wave/medium.wav"));
	if (gMedium == NULL) {
		printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return FALSE;
	}

	gLow = Mix_LoadWAV(FullPath("/default/wave/low.wav"));
	if (gLow == NULL) {
		printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return FALSE;
	}

	return TRUE;
}

void cleanMedia()
{
	if ( gScratch ) Mix_FreeChunk(gScratch),gScratch = NULL;
	if ( gHigh )    Mix_FreeChunk(gHigh),   gHigh = NULL;
	if ( gMedium )  Mix_FreeChunk(gMedium), gMedium = NULL;
	if ( gLow )     Mix_FreeChunk(gLow),    gLow = NULL;
	if ( gMusic )   Mix_FreeMusic(gMusic),  gMusic = NULL;
}

void initApp(int argc, char *argv[])
{
	SDL_SetHint(SDL_HINT_IME_INTERNAL_EDITING, "1" );
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS );
	// 创建窗口
	if( ! (gMainWindow = SDL_CreateWindow(argv[0], gMainWinRect.x, gMainWinRect.y, gMainWinRect.w, gMainWinRect.h, 0) ) ) {
		printf("Error create SDL window\n");
		cleanAll();	exit(-1);
	}
	// 创建绘制环境
	if( ! (gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED)) ) {
		printf("Error create SDL Renderer\n");
		cleanAll();	exit(-1);
	}
	// 初始化 true type font使用
	if ( TTF_Init() < 0 ) {
		printf("Couldn't initialize TTF: %s\n",SDL_GetError());
		cleanAll();	exit(-1);
	}
	//Initialize SDL_mixer
	if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 )
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		cleanAll();	exit(-1);
	}
	// load waves
	if( ! loadMedia() )	{
		cleanAll();	exit(-1);
	}
	memset(&uistate, 0, sizeof(uistate));
}

void endApp()
{
	cleanMedia();
	Mix_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(gMainRenderer);
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
}


void display()
{
	SDL_SetRenderDrawColor(gMainRenderer, gBackgroundColor.r, gBackgroundColor.g,
		gBackgroundColor.b, gBackgroundColor.a);
	SDL_RenderClear(gMainRenderer);
	// draw usage
	{
		int x = 20, y = 20;
		SDL_Color fc = {255,255,255,255};
		drawstring("Press 1,2,3,4 for sound effects", x, y, 
			FullPath("/default/FreeSerif.ttf"), 40, fc);
		drawstring("Press 9 to toggle music", x, y+=50, 
			FullPath("/default/FreeSerif.ttf"), 40, fc);
		drawstring("Press 0 to stop music", x, y+=50, 
			FullPath("/default/FreeSerif.ttf"), 40, fc);
		fc.r = 0;
		drawstring(uistate.music, x, y+=80, 
			FullPath("/default/FreeSerif.ttf"), 40, fc);
		fc.g = 0;
	}

	// draw a rect around the mouse
	drawrect(uistate.mousex-20, uistate.mousey-20, 40,40, 
		uistate.mousedown ? 0xFF00FF : 0xFFFF);
	// present the result
	SDL_RenderPresent(gMainRenderer);
}

int handleEvent(SDL_Event* e)
{
	switch (e->type) {
	case SDL_MOUSEMOTION:
		// update mouse position
		uistate.mousex = e->motion.x;
		uistate.mousey = e->motion.y; 
		return 1;
	case SDL_MOUSEBUTTONDOWN:
		// update button down state if left-clicking
		if (e->button.button == 1) {
			uistate.mousedown = 1;
			return 1;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		// update button down state if left-clicking
		if (e->button.button == 1) {
			uistate.mousedown = 0;
			return 1;
		}
		break;
	case SDL_KEYDOWN: 
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_1:
				//Play high sound effect
				Mix_PlayChannel(-1, gHigh, 0); 
				uistate.music = "music high";
				return 1;
			case SDLK_2:
				//Play medium sound effect
				Mix_PlayChannel(-1, gMedium, 0); 
				uistate.music = "music medium";
				return 1;
			case SDLK_3:
				//Play low sound effect
				Mix_PlayChannel(-1, gLow, 0);
				uistate.music = "music low";
				return 1;
			case SDLK_4:
				//Play scratch sound effect
				Mix_PlayChannel(-1, gScratch, 0);
				uistate.music = "music scratch";
				return 1;
			case SDLK_9:
				//If there is no music playing
				if (Mix_PlayingMusic() == 0) {
					//Play the music
					Mix_PlayMusic(gMusic, -1);
					uistate.music = "start playing music";
				} else { //music is being played	
					if (Mix_PausedMusic() == 1)
						Mix_ResumeMusic(); //Resume the music
					else
						Mix_PauseMusic();  //Pause the music
					uistate.music = "toggle playing music";
				}
				return 1;
			case SDLK_0:
				//Stop the music
				Mix_HaltMusic();
				uistate.music = "halt playing music";
				return 1;
			}
			return 0;
		}
		break;
	}
	// 没有处理事件
	return 0; 
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
			handleEvent( &e );
			// other events ...
		}
		// 做一些其他的事情。。。。。。。。
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

