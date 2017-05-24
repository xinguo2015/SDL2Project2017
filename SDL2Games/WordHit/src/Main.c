#include "hitword.h"

MineApp theApp;
 
char gMediaPath[256] = "";
extern char * MakePath(char path[], char dir[], char filename[]);
extern char *FullPath(char filename[]);

/************************************************************************/
/*函数原型：void InitApp(int argc,char* args[])							*/                                                                   
/*传入参数：计算提供到程序的参数										*/                                                                   
/*返 回 值：初始化是否成功												*/
/*函数功能：构建窗口与渲染器，加载媒体资源								*/                                                                   
/************************************************************************/
BOOL InitApp(int argc, char* args[])
{    
	memset(&theApp, 0, sizeof(theApp));
	theApp.SCREEN_WIDTH = 640;
	theApp.SCREEN_HEIGHT = 480;
	 
	//初始化SDL 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO )<0)
	{
		printf("SDL could not initialize! SDL error:%s\n",SDL_GetError());
		return FALSE;
	} 
	//创建游戏窗口
	theApp.gWindow=SDL_CreateWindow("Hitword               F1  to  stop / resume  music",
									SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, theApp.SCREEN_WIDTH, 
									theApp.SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if (theApp.gWindow==NULL)
	{
		printf("Fail to create SDL window! SDL error:%s\n",SDL_GetError());
		return FALSE;			
	}  
	//创建游戏窗口的渲染器
	theApp.gRenderer=SDL_CreateRenderer(theApp.gWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(theApp.gRenderer==NULL)
	{
		printf("Failed to create renderer!SDL Error:%s\n",SDL_GetError());
		return FALSE;
	}  
	//初始化PNG与JPG图片加载模式  
	if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG|IMG_INIT_JPG)))
	{
		printf( "IMG_INIT_PNG and IMG_INIT_JPG could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return FALSE;
	}
	//初始化TTF
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return FALSE;
	}

	//初始化SDL_mixer 
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
	//加载媒体资源
	if( ! LoadMedia())
	{
		printf("Failed in loading files\n");
		return FALSE;
	}
	
	postUserEvent(evtStartGame); 
	return TRUE;
}

/************************************************************************/
/*函数原型：void CleanApp()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：释放程序资源					 							*/                                                                   
/************************************************************************/
void CleanApp()
{ 
	int i;
	freeLTexture(&theApp.background.gback);  
	freeLTexture(&theApp.ufo.gufo);
	freeLTexture(&theApp.gbomb);
	freeLTexture(&theApp.gplanet); 
	freeLTexture(&theApp.gexplosion); 
	for (i=0;i<52;i++) 
		freeLTexture(&theApp.gletters[i]);  

	TTF_CloseFont( theApp.gFont ); 
	SDL_DestroyWindow(theApp.gWindow);
	SDL_DestroyRenderer(theApp.gRenderer);

	Mix_FreeMusic( theApp.gMusic ); 
	theApp.gMusic = NULL;
	Mix_FreeChunk( theApp.wavBomb );
	theApp.wavBomb = NULL;
	Mix_FreeChunk( theApp.wavFire ); 
	theApp.wavFire = NULL;

	
	Mix_Quit();
	TTF_Quit(); 
	IMG_Quit();
	SDL_Quit();
}

/************************************************************************/
/*函数原型：int main(int argc,char* args[]) 							*/                                                                   
/*传入参数：计算机传给程序的参数										*/                                                                   
/*返 回 值：是否正常退出												*/                                                                   
/*函数功能：入口函数,包含打字游戏的主流程			 					*/                                                                   
/************************************************************************/
int main( int argc, char* args[] )
{
	if( argc>1 ) 
		strcpy(gMediaPath, args[1]);
	else {
		strcpy(gMediaPath, SDL_GetBasePath());
		strcat(gMediaPath, "../");
	}
	printf("base path = %s\n", SDL_GetBasePath());
	printf("media path = %s\n", gMediaPath);

	if( ! InitApp(argc, args) )
		return 0; 
	RunApp();
	CleanApp();
	return 0;
}
