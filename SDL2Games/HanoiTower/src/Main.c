/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"

MineApp theApp;

char gMediaPath[256] = "";
extern char * MakePath(char path[], char dir[], char filename[]);
extern char *FullPath(char filename[]);

/************************************************************************/
/*函数原型：void setRects()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/
/*函数功能：设定塔与按钮的包围盒										*/                                                                   
/************************************************************************/
void setRects(){

	int i,x;

	//设定支架的包围盒
	MakeRect(&theApp.tower[0].rect,HX+towerWidth()/2-HW/2,HY,HW,towerHeight()-HW);
	MakeRect(&theApp.tower[1].rect,theApp.tower[0].rect.x+towerWidth()+HSPACE,HY,HW,towerHeight()-HW);
	MakeRect(&theApp.tower[2].rect,theApp.tower[1].rect.x+towerWidth()+HSPACE,HY,HW,towerHeight()-HW);
	 
	for (i=0;i<5;i++)
	{
		theApp.buttons[i].btnIndex=(BtnIndex)i;
		theApp.buttons[i].state=btnExposed;
		
		x=230+(btnWidth()+30)*i; 
		MakeRect(&theApp.buttons[i].rect,x,35,btnWidth(),btnHeight());
	}
}
 
/************************************************************************/
/*函数原型：void InitApp(int argc,char* args[])							*/                                                                   
/*传入参数：计算提供到程序的参数										*/                                                                   
/*返 回 值：初始化是否成功												*/
/*函数功能：初始化程序数据，加载媒体资源								*/                                                                   
/************************************************************************/
BOOL InitApp(int argc,char* args[]) {

	//初始化SDL 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO )<0)
	{
		printf("SDL could not initialize! SDL error:%s\n",SDL_GetError());
		return FALSE;
	} 
	//创建游戏窗口
	theApp.gWindow=SDL_CreateWindow("Hanoi",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
	  
	//加载媒体资源
	if (!LoadMedia())
	{
		printf("Failed in loading media!\n");
		return FALSE;
	}  
	initList();   
	setRects();
	postUserEvent(evtStartGame);

	return TRUE;  
}
 
/************************************************************************/
/*函数原型：void hanoi(int t1,int t2,int t3,int discs)					*/                                                                   
/*传入参数：塔t1,塔t2,塔t3,移动圆盘数discs								*/                                                                   
/*返 回 值：无															*/
/*函数功能：递归实现从t1移动discs个圆盘至t3								*/                                                                   
/************************************************************************/
void hanoi(int t1,int t2,int t3,int discs){
	 
	if (discs==1) 
		moves(t1,t3); 
	else
	{
		hanoi(t1,t3,t2,discs-1);
		moves(t1,t3);
		hanoi(t2,t1,t3,discs-1);
	} 
}

/************************************************************************/
/*函数原型：int threadFunction( void* data )							*/                                                                   
/*传入参数：SDL线程标准入参												*/                                                                   
/*返 回 值：0															*/
/*函数功能：独立线程执行汉诺塔递归更新数据								*/                                                                   
/************************************************************************/
int threadFunction( void* data )
{ 
	hanoi(0,1,2,theApp.discs);

	return 0;
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
	freeLTexture(&theApp.bgLT);  
	freeLTexture(&theApp.towerLT);
	freeLTexture(&theApp.discsLT); 
	freeLTexture(&theApp.btnLT);
	freeLTexture(&theApp.winLT);
	for (i=0;i<10;i++)
	{
		freeLTexture(&theApp.numbersLT[i]);
	}

	clearNodes();
	  
	TTF_CloseFont( theApp.gFont ); 
	SDL_DestroyWindow(theApp.gWindow);
	SDL_DestroyRenderer(theApp.gRenderer);
	   
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
