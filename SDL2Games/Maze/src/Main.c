/****************************************************************** 
File name: Main.c
Date: 2015/8/7
Description:	主文件 
				迷宫生成算法参考//http://dourok.info/2011/07/14/maze-generation-algorithm/
*******************************************************************/
#include "maze.h"

char gMediaPath[256] = "../Games/HanoiTower";
extern char * MakePath(char path[], char dir[], char filename[]);
extern char *FullPath(char filename[]);
MineApp theApp; 

  
/************************************************************************/
/*函数原型：void CalcLayout()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：计算游戏计时器,按钮的Rect						 			*/                                                                   
/************************************************************************/
void CalcLayout()
{  
	int x,y;
	
	//计时器位于X方向的中心
	x=SCREEN_WIDTH/2-btnW()/2;
	y=partH()/2-btnH()/2;
	MakeRect(&theApp.timeBtn.rect, x, y, btnW(), btnH());
	
	//自动演示按钮位于三个按钮的中间
	x=SCREEN_WIDTH*3/4-btnW()*3/4-3;
	MakeRect(&theApp.autoBtn.rect, x, y, btnW(), btnH()); 

	
	//加速播放按钮在自动演示按钮左侧
	x=SCREEN_WIDTH*3/4-btnW()*3/4-13;
	MakeRect(&theApp.upBtn.rect, x, y, btnW()/2, btnH()); 

	//减速播放按钮在自动演示按钮右侧
	x=SCREEN_WIDTH*3/4+7;
	MakeRect(&theApp.downBtn.rect, x, y, btnW()/2, btnH()); 
	

	//手动操作按钮位于屏幕右端
	x=SCREEN_WIDTH-btnW()-6;
	MakeRect(&theApp.manualBtn.rect, x, y, btnW(), btnH());
}

/************************************************************************/
/*函数原型：BOOL goMaze(int r,int c)									*/                                                                   
/*传入参数：南瓜所在位置对应GameBoard数组的行号，列号					*/                                                                   
/*返 回 值：当前位置在通往出口的路径上返回TRUE,否则返回FALSE			*/
/*函数功能：迷宫递归寻路函数											*/                                                                   
/************************************************************************/
BOOL goMaze(int r,int c)
{    
	theApp.GameBoard[r][c]=STEP;
	//到达迷宫出口
	if (IsSucceed()) 
	{ 
		theApp.OnTheWay=TRUE; 
	}
		 
	//判断向上是否有路
	if (!theApp.OnTheWay && theApp.GameBoard[r-1][c]==BLANK)
	{ 
		//向上移动一个位置
		moveUp(r*partH());//因为屏幕第一行为工具栏，所以Gameboard数组的第r-1行代表屏幕第r行
		if (!goMaze(r-1,c))
		{ //如果上方位置不在正确路劲中，向下回退一个位置
			moveDown((r+1)*partH());//同理，Gameboard数组的第r行代表屏幕第r+1行
		} 
	}  
	//判断向下是否有路
	if (!theApp.OnTheWay && theApp.GameBoard[r+1][c]==BLANK)
	{ 
		moveDown((r+2)*partH());//同理，Gameboard数组的第r+1行代表屏幕第r+2行
		if (!goMaze(r+1,c))
		{
			//如果下方位置不在正确路劲中，向上回退一个位置
			moveUp((r+1)*partH());
		} 
	} 
	//判断向右是否有路
	if (!theApp.OnTheWay && theApp.GameBoard[r][c+1]==BLANK)
	{ 
		moveRight((c+1)*partW());
		if (!goMaze(r,c+1))
		{ //如果右方位置不在正确路劲中，向左回退一个位置
			moveLeft(c*partW());
		} 
	}  
	//判断向左是否有路
	if (!theApp.OnTheWay && theApp.GameBoard[r][c-1]==BLANK)
	{ 
		moveLeft((c-1)*partW());
		if (!goMaze(r,c-1))
		{ //如果左方位置不在正确路劲中，向右回退一个位置
			moveRight(c*partW());
		} 
	}  
	
	//此位置不在通往出口的路径上
	if(!theApp.OnTheWay)
		theApp.GameBoard[r][c]=BLANK;

	return theApp.OnTheWay;
}
 
/************************************************************************/
/*函数原型：int threadFunction( void* data )							*/                                                                   
/*传入参数：SDL线程标准入参												*/                                                                   
/*返 回 值：0															*/
/*函数功能：独立线程执行迷宫递归寻路									*/                                                                   
/************************************************************************/
int threadFunction( void* data )
{	
	int row,col; 
	row=pumpkinY()/partH()-1;  
	col=pumpkinX()/partW();		 
	goMaze(row,col); 
	return 0;
}
 
/************************************************************************/
/*函数原型：void InitApp(int argc,char* args[])							*/                                                                   
/*传入参数：计算提供到程序的参数										*/                                                                   
/*返 回 值：初始化是否成功												*/
/*函数功能：初始化程序数据，加载媒体资源								*/                                                                   
/************************************************************************/
BOOL InitApp(int argc,char* args[]) {
	
	memset(&theApp, 0, sizeof(theApp));
	//初始化SDL 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO )<0)
	{
		printf("SDL could not initialize! SDL error:%s\n",SDL_GetError());
		return FALSE;
	} 
	//创建游戏窗口
	theApp.gWindow=SDL_CreateWindow("Maze",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
	 
	LoadMedia(); //加载媒体资源
	CalcLayout();//设定计时器与按钮的位置和大小
	postUserEvent(evtStartGame);
	return TRUE; 
}
  
/************************************************************************/
/*函数原型：void CleanApp()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无								 							*/                                                                   
/*函数功能：释放程序资源					 							*/                                                                   
/************************************************************************/
void CleanApp()
{     
	int i=0;
	//释放程序中加载的纹理
	freeLTexture(&theApp.btnLT); 
	freeLTexture(&theApp.partLT); 
	freeLTexture(&theApp.successLT[0]);
	freeLTexture(&theApp.successLT[1]);
	freeLTexture(&theApp.failLT[0]);
	freeLTexture(&theApp.failLT[1]);
	freeLTexture(&theApp.levelLT);
	for (i=0;i<10;i++)
	{
		freeLTexture(theApp.numbersLT+i);
	}
	//释放SDL_Window与SDL_Renderer
	SDL_DestroyWindow(theApp.gWindow);
	SDL_DestroyRenderer(theApp.gRenderer);
	theApp.gWindow=NULL;
	theApp.gRenderer=NULL;

	TTF_Quit(); 
	IMG_Quit();
	SDL_Quit();
}

/************************************************************************/
/*函数原型：int main(int argc,char* args[]) 							*/                                                                   
/*传入参数：计算机传给程序的参数										*/                                                                   
/*返 回 值：是否正常退出												*/                                                                   
/*函数功能：入口函数,包含汉诺塔的主流程									*/                                                                   
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
