/****************************************************************** 
File name: Util.c
Date: 2015/8/8
Description: 工具类函数 
*******************************************************************/
#include "maze.h"
 
/************************************************************************/
/*函数原型：void MakeRect(SDL_Rect *rect,int x,int y,int w,int h)		*/                                                                   
/*传入参数：待修改的SDL_Rect，横坐标x,纵坐标y，宽度w，高度h				*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：设定入参SDL_Rect的坐标与宽高								*/                                                                   
/************************************************************************/
void MakeRect(SDL_Rect *rect,int x,int y,int w,int h){

	rect->x=x;
	rect->y=y;
	rect->w=w;
	rect->h=h; 
}

/************************************************************************/
/*函数原型：BOOL checkCollision( SDL_Rect a, SDL_Rect b )				*/                                                                   
/*传入参数：矩形包围盒a,矩形包围盒b										*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：判断包围盒a与包围盒b是否碰撞								*/                                                                   
/************************************************************************/
BOOL checkCollision( SDL_Rect a, SDL_Rect b ){

	//矩形包围盒的边界
	int leftA,leftB; 
	int rightA,rightB; 
	int topA,topB; 
	int buttomA,buttomB; 

	//计算 rect A的边界
	leftA=a.x;
	rightA=a.x+a.w;
	topA=a.y;
	buttomA=a.y+a.h;

	//计算 rect B的边界
	leftB=b.x;
	rightB=b.x+b.w;
	topB=b.y;
	buttomB=b.y+b.h;

	//如果矩形A有边界在B之外
	if (rightA<=leftB)
	{
		return FALSE;
	}
	if (leftA>=rightB)
	{
		return FALSE;
	}
	if (buttomA<=topB)
	{
		return FALSE;
	}
	if (topA>=buttomB)
	{
		return FALSE;
	} 
	 
	return TRUE;
}
 
/************************************************************************/
/*函数原型：void setDigitWH()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无										   					*/                                                                   
/*函数功能：设定个位数纹理宽度高度						 				*/                                                                   
/************************************************************************/
void setDigitWH(){
	int maxw=0,maxh=0,i;
	//获取0-9十个数字纹理中宽度与高度的最大值
	for (i=1;i<10;i++)
	{
		if (theApp.numbersLT[i].mWidth>theApp.numbersLT[maxw].mWidth)
		{
			maxw=i;
		}
		if (theApp.numbersLT[i].mHeight>theApp.numbersLT[maxh].mHeight)
		{
			maxh=i;
		}
	}
	//设定数字纹理为最大值
	theApp.DigitW=theApp.numbersLT[maxw].mWidth;
	theApp.DigitH=theApp.numbersLT[maxh].mHeight;
}
  
extern char gMediaPath[256];

char * MakePath(char path[], char dir[], char filename[])
{
	strcpy(path,dir);
	return strcat(path, filename);
}

char *FullPath(char filename[])
{
	static char path[256];
	return MakePath(path, gMediaPath, filename);
}

/************************************************************************/
/*函数原型：BOOL LoadMedia()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：媒体全部加载成功返回TRUE,否则返回FALSE   					*/                                                                   
/*函数功能：加载游戏中用到的媒体资源					 				*/                                                                   
/************************************************************************/
BOOL LoadMedia(){
	 int i;  
	//加载界面部件纹理
	theApp.partLT=load_image(FullPath("/media/part.png"));
	if (!theApp.partLT.mTexture)
	{
		printf("Failed to load parts.png.\n");
		return FALSE;
	}    
	theApp.npartSprite=3;

	//加载按钮纹理
	theApp.btnLT=load_image(FullPath("/media/button.png"));
	if (!theApp.btnLT.mTexture)
	{
		printf("Failed to load buttons.png.\n");
		return FALSE;
	}    
	theApp.nbtnSprite=4;

	//加载数字纹理
	theApp.gFont = TTF_OpenFont( FullPath("/media/Digit.ttf"), 24 );
	for (i=0;i<10;i++)
	{
		theApp.numbersLT[i]=load_Number(i);
	}
	setDigitWH();
	
	//加载游戏结束提示标语
	theApp.gFont = TTF_OpenFont( FullPath("/media/Semitalic.ttf"), 80 ); 
	theApp.successLT[0]=load_TTF_text("Success",theApp.gFont); 
	theApp.failLT[0]=load_TTF_text("Fail",theApp.gFont); 

	theApp.gFont = TTF_OpenFont( FullPath("/media/ingleby.ttf"), 24 ); 
	theApp.successLT[1]=load_TTF_text("Please press SPACE to next level or ESC to quit.",theApp.gFont); 
	theApp.failLT[1]=load_TTF_text("Please press SPACE to restart or ESC to quit.",theApp.gFont); 
	
	//加载游戏等级提示
	theApp.levelLT=load_TTF_text("Level:",theApp.gFont); 


	return TRUE; 
}
 
/************************************************************************/
/*函数原型：BOOL IsSucceed()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：南瓜抵达目的地返回TRUE,否则返回FALSE	   					*/                                                                   
/*函数功能：判断游戏是否成功							 				*/                                                                   
/************************************************************************/
BOOL IsSucceed(){

	if (checkCollision(theApp.pumpkin.rect,theApp.exitRect))
	{
		//如果到达出口则任务完成
		return TRUE;
	}

	return FALSE;
}

/************************************************************************/
/*函数原型：BOOL GameOver()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：游戏胜利或者游戏失败返回TRUE,否则返回FALSE 					*/                                                                   
/*函数功能：判断游戏结束								 				*/                                                                   
/************************************************************************/
BOOL GameOver(){

	if (theApp.gamestate==sGameFail || theApp.gamestate==sGameWin)
	{
		return TRUE;
	}
	return FALSE;

}

/************************************************************************/
/*函数原型：int btnW()													*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：单个按钮纹理的宽度											*/                                                                   
/*函数功能：获取单个按钮纹理的宽度										*/                                                                   
/************************************************************************/
int btnW(){

	return theApp.btnLT.mWidth;
}   

/************************************************************************/
/*函数原型：int btnH()													*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：单个按钮纹理的高度											*/                                                                   
/*函数功能：获取单个按钮纹理的高度										*/                                                                   
/************************************************************************/
int btnH(){

	return theApp.btnLT.mHeight/theApp.nbtnSprite;
}

/************************************************************************/
/*函数原型：void * GetFocus()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：获取焦点部件的指针											*/                                                                   
/************************************************************************/
void * GetFocus()
{
	return theApp.pFocus;
}

/************************************************************************/
/*函数原型：void * SetFocus(void * pCtrl)								*/                                                                   
/*传入参数：空类型的指针												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定焦点为pCtrl 											*/                                                                   
/************************************************************************/
void * SetFocus(void * pCtrl)
{
	theApp.pFocus = pCtrl;
	return pCtrl;
}

/************************************************************************/
/*函数原型：BOOL HitRectTest(int x, int y, SDL_Rect rect)				*/                                                                   
/*传入参数：待修改的SDL_Rect，横坐标x,纵坐标y，宽度w，高度h				*/                                                                   
/*返 回 值：坐标位于矩形区域内返回TRUE，否则返回FALSE					*/                                                                   
/*函数功能：判断坐标（x，y）是否位于矩形区域内							*/                                                                   
/************************************************************************/
BOOL HitRectTest(int x, int y, SDL_Rect rect)
{
	return (x>=rect.x && y>=rect.y && x<rect.x+rect.w && y<rect.y+rect.h );
}

/************************************************************************/
/*函数原型：void* GetControlAt(int x,int y)								*/                                                                   
/*传入参数：横坐标x,纵坐标y												*/                                                                   
/*返 回 值：坐标所在的部件						   						*/                                                                   
/*函数功能：获取屏幕坐标(x,y)所在的按钮									*/                                                                   
/************************************************************************/
void* GetControlAt(int x,int y){
	 
	void* pFocus=NULL;
	
	if( HitRectTest(x,y,theApp.manualBtn.rect))
		pFocus = &theApp.manualBtn;

	if( !theApp.autoRun && HitRectTest(x,y,theApp.autoBtn.rect))
		 pFocus = &theApp.autoBtn;
	  
	if( theApp.autoRun && HitRectTest(x,y,theApp.upBtn.rect))
		pFocus = &theApp.upBtn;

	if( theApp.autoRun && HitRectTest(x,y,theApp.downBtn.rect))
		pFocus = &theApp.downBtn;

	return pFocus;
}
  
