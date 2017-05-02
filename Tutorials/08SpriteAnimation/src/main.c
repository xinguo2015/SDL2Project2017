#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "utilities.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

#define WALKING_ANIMATION_FRAMES 4
#define FALSE 0  
#define TRUE  1 
typedef int BOOL;

char           gMediaPath[256] = "../Media";
SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
//Screen dimension constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480; 
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
SDL_Color      gBackgroundColor = { 0, 0, 0, 255 };

Mix_Music *gBackgroundMusic = NULL;

typedef struct { 
	SDL_Texture* mTexture;  
	int mWidth;
	int mHeight;
}LTexture;

//Loads individual image as texture
LTexture loadTexture( char* path ); 
//Free LTexture
void freeLTexture(LTexture ltexture);
//Render texture 
void renderTexture(LTexture ltexture, int x, int y, SDL_Rect* clip);

//Background texture
LTexture gBackgroundTexture;
//Displayed texture
LTexture gSpriteTexture;  
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

void freeLTexture(LTexture ltexture)
{
	if( ltexture.mTexture )
	{ 
		SDL_DestroyTexture(ltexture.mTexture);
		ltexture.mTexture=NULL;
		ltexture.mWidth=0;
		ltexture.mHeight=0;
	} 
}

void cleanAll()
{
	//Free image 
	freeLTexture(gSpriteTexture);
	freeLTexture(gBackgroundTexture);
	// free music
	if( gBackgroundMusic ) Mix_FreeMusic( gBackgroundMusic );
	gBackgroundMusic = NULL;
	if( gMainRenderer ) SDL_DestroyRenderer(gMainRenderer);
	gMainRenderer = NULL;
	if( gMainWindow ) SDL_DestroyWindow(gMainWindow);
	gMainWindow = NULL;
}

BOOL loadMedia()
{
	// load a back ground music
	gBackgroundMusic = Mix_LoadMUS( FullPath("/default/wave/bgmusic.mp3") );
	if( gBackgroundMusic == NULL )
	{
		printf( "Failed to load bgmusic music! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
	//Load PNG texture
	gBackgroundTexture = loadTexture( FullPath("/default/background.png") ); 
	if( gBackgroundTexture.mTexture == NULL )
	{
		printf( "Failed to load background image!\n" );
		return FALSE;
	}  

	gSpriteTexture = loadTexture( FullPath("/default/animation.png") ); 
	if( gSpriteTexture.mTexture == NULL )
	{
		printf( "Failed to load foo image!\n" );
		return FALSE;
	}  
	//Set sprite clips
	gSpriteClips[ 0 ].x =   0;
	gSpriteClips[ 0 ].y =   0;
	gSpriteClips[ 0 ].w =  64;
	gSpriteClips[ 0 ].h = 205;

	gSpriteClips[ 1 ].x =  64;
	gSpriteClips[ 1 ].y =   0;
	gSpriteClips[ 1 ].w =  64;
	gSpriteClips[ 1 ].h = 205;

	gSpriteClips[ 2 ].x = 128;
	gSpriteClips[ 2 ].y =   0;
	gSpriteClips[ 2 ].w =  64;
	gSpriteClips[ 2 ].h = 205;

	gSpriteClips[ 3 ].x = 196;
	gSpriteClips[ 3 ].y =   0;
	gSpriteClips[ 3 ].w =  64;
	gSpriteClips[ 3 ].h = 205;

	return TRUE;
}

LTexture loadTexture(char* path)
{
	LTexture ltexture = {0,0,0};
	//Load image at specified path
	SDL_Surface* loadedSurface=IMG_Load(path);
	if(loadedSurface==NULL)
	{
		printf("Unable to load image %s!SDL Error: %s\n",path,SDL_GetError());
		return ltexture;
	} 
	SDL_SetColorKey( loadedSurface, SDL_TRUE, 
		SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
	//Create texture from surface pixels  
	ltexture.mTexture=SDL_CreateTextureFromSurface(gMainRenderer,loadedSurface);
	SDL_FreeSurface(loadedSurface);
	if (ltexture.mTexture==NULL)
	{ 
		printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		return ltexture;
	} 
	//Get image dimensions
	ltexture.mWidth=loadedSurface->w;
	ltexture.mHeight=loadedSurface->h;

	return ltexture;	
}

void renderTexture(LTexture ltexture, int x, int y, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect dstrect = { x, y, ltexture.mWidth, ltexture.mHeight };
	//Set clip rendering dimensions
	if ( clip )	{
		dstrect.w = clip->w;
		dstrect.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopy(gMainRenderer, ltexture.mTexture, clip, &dstrect);
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

	//Initialize PNG loading 
	if( !( IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		cleanAll(); exit(-1);
	}	
	//初始化SDL_mixer 
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		cleanAll(); exit(-1);
	}
}

void endApp()
{
	cleanAll();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void display()
{
	static int currentFrame = 0;
	SDL_Rect* currentClip; 
	//Render background
	SDL_RenderCopy(gMainRenderer,gBackgroundTexture.mTexture,NULL,NULL);
	//Render sprite
	currentClip = &gSpriteClips[ currentFrame / 8 ];
	renderTexture(gSpriteTexture, (SCREEN_WIDTH - currentClip->w) / 2, 
		(SCREEN_HEIGHT - currentClip->h) / 2, currentClip);
	//Go to next frame
	++currentFrame; 
	//Cycle animation
	if( currentFrame / 8 >= WALKING_ANIMATION_FRAMES )	{
		currentFrame = 0;
	}
	SDL_RenderPresent(gMainRenderer);
}

void handleEvent(SDL_Event* e)
{
	switch (e->type) {
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_KEYDOWN:
		break;
	case SDL_KEYUP:
		switch (e->key.keysym.sym) {
		case SDLK_ESCAPE: 
			{
				SDL_Event ev;
				ev.type = SDL_QUIT; 
				SDL_PushEvent( &ev );
			}
		}
		break;
	default: 
		break;
	}
}


int main(int argc, char *argv[]) 
{
	BOOL gameover = FALSE;
	if( argc>1 ) strcpy(gMediaPath, argv[1]);
	initApp(argc,argv);
	if( !loadMedia() ) {
		cleanAll(); exit(-1);
	}
	if( gBackgroundMusic && Mix_PlayingMusic() == 0 ) 
		Mix_PlayMusic( gBackgroundMusic, -1 ); 
	// Enter event processing loop 
	while ( ! gameover ) 
	{
		SDL_Event e;
		if ( ! gameover && SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameover = TRUE; // 终止应用程序
			}
			handleEvent( &e );
		}
		display();
		SDL_Delay(10); 
	}
	endApp();

	return 0;
}

