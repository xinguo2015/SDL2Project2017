/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "miner.h"
 
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
	
	int fldSizeX = 30;
	int fldSizeY = 16;
	int bombCount= 99;
	
	/* small for debuging
	fldSizeX = 10;
	fldSizeY = 6; 
	bombCount = 5; 
	*/
	int mm = 16; // margin size
	int sz = 16; // bomb size
	int h1 = 25; // first row height

	memset(&theApp, 0, sizeof(theApp));
	theApp.SCREEN_WIDTH = mm*2 + fldSizeX * sz;
	theApp.SCREEN_HEIGHT = mm*3 + h1 + fldSizeY * sz;

	
	if( !ConstructField(&theApp.mineField, fldSizeX, fldSizeY, bombCount) )
		return FALSE;

	//初始化SDL 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO  | SDL_INIT_TIMER )<0)
	{
		printf("SDL could not initialize! SDL error:%s\n",SDL_GetError());
		return FALSE;
	} 
	//创建游戏窗口
	theApp.gWindow=SDL_CreateWindow("Miner",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, theApp.SCREEN_WIDTH, theApp.SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
	//初始化SDL_mixer 
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 512 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
	//加载媒体资源
	if( ! LoadMedia() )
	{
		printf("Failed in loading files\n");
		return FALSE;
	}
	  
	CalcLayout(); 
  
	postUserEvent(evtNewGame); 
	return TRUE;
}

/************************************************************************/
/*函数原型：void InitMineField()										*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：初始化地雷的位置，数量										*/                                                                   
/************************************************************************/
void InitMineField()
{
	GenerateRandomBombs(& theApp.mineField);
	CalcNeighbors(& theApp.mineField);
	theApp.mineField.cellSize[0] = CellsWidth();
	theApp.mineField.cellSize[1] = CellsHeight();
	theApp.mineField.state = sNewGame;
	theApp.mineField.pFocus = NULL;
}

/************************************************************************/
/*函数原型：void StartGame()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：开始新一局游戏时进行数据初始化								*/                                                                   
/************************************************************************/
void StartGame(){

	SDL_RemoveTimer(theApp.timerID);
	theApp.timerID = NULL;
	InitMineField();
	theApp.timeButton.data = 0;
	theApp.taskButton.data = theApp.mineField.bombCount;
	theApp.faceButton.data = FaceSmile;
	theApp.winNotPlay=TRUE;
}

/************************************************************************/
/*函数原型：void CleanApp()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：释放程序资源					 							*/                                                                   
/************************************************************************/
void CleanApp()
{
	Mix_FreeChunk( theApp.audio_start );
	theApp.audio_start = NULL;
	Mix_FreeChunk( theApp.audio_bomb );
	theApp.audio_bomb = NULL;
	Mix_FreeChunk( theApp.audio_reveal );
	theApp.audio_reveal = NULL;
	Mix_FreeChunk( theApp.audio_win );
	theApp.audio_win = NULL;
	 
	freeLTexture(&theApp.imgBombs);
	freeLTexture(&theApp.imgFaces);
	freeLTexture(&theApp.imgDigits); 
	
	SDL_DestroyWindow(theApp.gWindow);
	SDL_DestroyRenderer(theApp.gRenderer);
	 
	IMG_Quit(); 
	Mix_Quit();
	SDL_Quit();

	DestroyField(&theApp.mineField);
	 
}

/************************************************************************/
/*函数原型：int main(int argc,char* args[]) 							*/                                                                   
/*传入参数：计算机传给程序的参数										*/                                                                   
/*返 回 值：是否正常退出												*/                                                                   
/*函数功能：入口函数,包含扫雷游戏的主流程			 					*/                                                                   
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
