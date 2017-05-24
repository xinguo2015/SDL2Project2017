#include "Russia.h" 	
 
char gMediaPath[256] = "";
extern char * MakePath(char path[], char dir[], char filename[]);
extern char *FullPath(char filename[]);

MineApp theApp;

Block arrayBLOCK[19]={ 
	/* x1,y1,x2,y2,x3,y3,x4,y4,boxtype,next*/
	{0,-2,0,-1,0,0,1,0,BOX_BLUE,1},		/**第一种形状**/
	{-1,0,0,0,1,-1,1,0,BOX_BLUE,2},
	{0,-2,1,-2,1,-1,1,0,BOX_BLUE,3},
	{-1,-1,-1,0,0,-1,1,-1,BOX_BLUE,0},

	{0,-2,0,-1,0,0,1,-2,BOX_PINK,5},	/**第二种形状**/
	{-1,-1,-1,0,0,0,1,0,BOX_PINK,6},
	{0,0,1,-2,1,-1,1,0,BOX_PINK,7},
	{-1,-1,0,-1,1,-1,1,0,BOX_PINK,4},

	{-1,0,0,-1,0,0,1,0,BOX_YELLOW,9},	/**第三种形状**/
	{-1,-1,0,-2,0,-1,0,0,BOX_YELLOW,10},
	{-1,-1,0,-1,0,0,1,-1,BOX_YELLOW,11},
	{0,-2,0,-1,0,0,1,-1,BOX_YELLOW,8},

	{-1,0,0,-1,0,0,1,-1,BOX_GREEN,13},	/**第四种形状**/
	{0,-2,0,-1,1,-1,1,0,BOX_GREEN,12},

	{-1,-1,0,-1,0,0,1,0,BOX_RED,15},	/**第五种形状**/
	{0,-1,0,0,1,-2,1,-1,BOX_RED,14},

	{0,-3,0,-2,0,-1,0,0,BOX_ORANGE,17},	/**第六种形状**/
	{-1,0,0,0,1,0,2,0,BOX_ORANGE,16},

	{0,-1,0,0,1,-1,1,0,BOX_CYAN,18}		/**第七种形状**/

};


/************************************************************************/
/*函数原型：void CalcLayout()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定游戏中按钮，等级和计分的rect			 				*/                                                                   
/************************************************************************/
void CalcLayout(){
	
	MakeRect(&theApp.StartBtn.rect,400,290,ButtonWidth(),ButtonHeight());
	MakeRect(&theApp.StopBtn.rect,400,345,ButtonWidth(),ButtonHeight());
	MakeRect(&theApp.QuitBtn.rect,400,400,ButtonWidth(),ButtonHeight()); 
}

/************************************************************************/
/*函数原型：void InitApp(int argc,char* args[])							*/                                                                   
/*传入参数：计算提供到程序的参数										*/                                                                   
/*返 回 值：初始化是否成功												*/
/* 函数功能：初始化游戏数据，建立窗口与渲染器，设定按钮，计分的位置		*/                                                                   
/************************************************************************/
BOOL InitApp(int argc,char* args[]){
	  
	 //初始化SDL 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO )<0)
	{
		printf("SDL could not initialize! SDL error:%s\n",SDL_GetError());
		return FALSE;
	} 
	//创建游戏窗口
	theApp.gWindow=SDL_CreateWindow("Tetris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
	//采用PNG和JPG图片加载模式 
	if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG|IMG_INIT_JPG)))
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return FALSE;
	}
	//初始化SDL_ttf
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		return FALSE;
	}

	//初始化SDL_mixer 
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 512 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
	  
	//加载媒体资源
	if (!LoadMedia())
	{
		printf("Failed in loading media!\n");
		return FALSE;
	}
	 
	//设定按钮，计分和难度等级的rect
	CalcLayout();
	theApp.gamestate=NotStart;
	return TRUE; 
	
}

/************************************************************************/
/*函数原型：void StartGame()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/
/* 函数功能：每局游戏开始前初始化游戏状态，等级，分数等数据				*/                                                                   
/************************************************************************/
void StartGame(){

	int i,j; 
	/*初始化代表游戏面板的数组*/
	for (j=0;j<=BOARD_HEIGHT;j++)
		for (i=0;i<BOARD_WIDTH+2;i++)
		{
			if (i==0 || i==BOARD_WIDTH+1)
				theApp.Gameboard[i][j]=BOX_BLUE;
			else
				theApp.Gameboard[i][j]=BOX_BLANK;
		}
	for (i=0;i<BOARD_WIDTH+2;i++)
		theApp.Gameboard[i][BOARD_HEIGHT+1]=BOX_BLUE;
	  
	/*初始化得分,等级和下落形状的数据*/ 
	resetLevel();
	resetScore(); 
	resetShape();
}

/************************************************************************/
/*函数原型：void CleanApp()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：释放程序资源					 							*/                                                                   
/************************************************************************/
void CleanApp(){
	 
	Mix_FreeChunk( theApp.audio_attach );
	theApp.audio_attach = NULL;
	Mix_FreeChunk( theApp.audio_score );
	theApp.audio_score = NULL;
	Mix_FreeChunk( theApp.audio_levelUp );
	theApp.audio_levelUp = NULL; 
	Mix_FreeChunk( theApp.audio_btnDown );
	theApp.audio_btnDown = NULL; 
	Mix_FreeChunk( theApp.audio_fail );
	theApp.audio_fail = NULL;

	freeLTexture(&theApp.StartBtn.ltexture);
	freeLTexture(&theApp.StopBtn.ltexture);
	freeLTexture(&theApp.QuitBtn.ltexture);
	freeLTexture(&theApp.imgBackground);
	freeLTexture(&theApp.imgBox);
	freeLTexture(&theApp.Score.ltexture);
	freeLTexture(&theApp.Level.ltexture);

	TTF_CloseFont( theApp.gFont ); 
	SDL_DestroyWindow(theApp.gWindow);
	SDL_DestroyRenderer(theApp.gRenderer);
	theApp.gFont = NULL;
	theApp.gWindow=NULL;
	theApp.gRenderer=NULL;
	
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

/************************************************************************/
/*函数原型：int main(int argc,char* args[]) 							*/                                                                   
/*传入参数：计算机传给程序的参数										*/                                                                   
/*返 回 值：是否正常退出												*/                                                                   
/*函数功能：入口函数,包含俄罗斯方块程序的主流程			 				*/                                                                   
/************************************************************************/
int main(int argc,char* args[])
{   
	if( argc>1 ) 
		strcpy(gMediaPath, args[1]);
	else {
		strcpy(gMediaPath, SDL_GetBasePath());
		strcat(gMediaPath, "../");
	}
	printf("base path = %s\n", SDL_GetBasePath());
	printf("media path = %s\n", gMediaPath);
	   
	if (!InitApp(argc,args))
	{  
		printf("Fail to InitApp!\n");
		return FALSE;
	}   
	RunApp();  
	CleanApp();  
	return 0; 
}

