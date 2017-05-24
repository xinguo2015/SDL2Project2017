/****************************************************************** 
File name: Util.c
Date: 2015/7/28
Description: 通用工具类函数 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"

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
/*函数功能：获取屏幕坐标所在的部件（按钮，圆盘或者空）					*/                                                                   
/************************************************************************/
void* GetControlAt(int x,int y){

	int i;
	void* pFocus=NULL;
	 
	for (i=0;i<5;i++)
	{
		if( HitRectTest(x,y,theApp.buttons[i].rect))
			return theApp.buttons+i;
	}

	pFocus=HitDiscTest(x,y); 

	return pFocus;
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
	 SDL_Color textcolor={255,0,0,255};
	 
	 //加载游戏成功纹理
	theApp.gFont = TTF_OpenFont( FullPath("/media/Arial.ttf"), 50 ); 
	theApp.winLT=load_TTF_text("Succeed",theApp.gFont,textcolor);
		  
	//加载游戏背景纹理
	theApp.bgLT=load_image(FullPath("/media/pics/background.jpg"));
	if (!theApp.bgLT.mTexture)
	{
		printf("Failed to load background.\n");
		return FALSE;
	}    
	

	//加载数字纹理
	theApp.gFont = TTF_OpenFont( FullPath("/media/Arial.ttf"), 18 );
	for (i=0;i<10;i++)
	{
		theApp.numbersLT[i]=load_Number(i);
	}

	//加载底座纹理
	theApp.towerLT=load_image( FullPath("/media/pics/tower.png"));
	if (!theApp.towerLT.mTexture)
	{
		printf("Failed to load towerLT.\n");
		return FALSE;
	}   
	 
	//加载圆盘纹理
	theApp.discsLT=load_image(FullPath("/media/pics/discs.png"));
	if (!theApp.discsLT.mTexture)
	{
		printf("Failed to load discs.\n");
		return FALSE;
	} 
	theApp.ndiscSprite=9;
	setDiscs(3);

	//加载按钮纹理
	theApp.btnLT=load_image(FullPath("/media/pics/button.png"));
	if (!theApp.btnLT.mTexture)
	{
		printf("Failed to load button.\n");
		return FALSE;
	} 
	theApp.btnRow=2;
	theApp.btnCol=5;
	 
	return TRUE; 
}

/************************************************************************/
/*函数原型：BOOL IsBeginning()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：所有disc处于第一列返回TRUE,否则返回FALSE   					*/                                                                   
/*函数功能：判断游戏是否处于初始状态					 				*/                                                                   
/************************************************************************/
BOOL IsBeginning(){

	if (theApp.tower[0].header->index==theApp.discs)
		return TRUE;

	return FALSE; 
}
 
/************************************************************************/
/*函数原型：BOOL IsWinning()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：所有disc处于第二列或第三列返回TRUE,否则返回FALSE   			*/                                                                   
/*函数功能：判断游戏是否成功							 				*/                                                                   
/************************************************************************/
BOOL IsWinning(){

	if (theApp.tower[1].header->index==theApp.discs || theApp.tower[2].header->index==theApp.discs)
		return TRUE;

	return FALSE; 
}

/************************************************************************/
/*函数原型：int towerCenX(int t)										*/                                                                   
/*传入参数：塔t															*/                                                                   
/*返 回 值：无										   					*/                                                                   
/*函数功能：从塔t中心的X坐标							 				*/                                                                   
/************************************************************************/
int towerCenX(int t){

	return theApp.tower[t].rect.x+HW/2; 

}
  
/************************************************************************/
/*函数原型：int towerCenY(int t)										*/                                                                   
/*传入参数：塔t															*/                                                                   
/*返 回 值：无										   					*/                                                                   
/*函数功能：从塔t最小圆盘底部的Y坐标							 		*/                                                                   
/************************************************************************/
int towerCenY(int t){

	return HY+towerHeight()-HW-discHeight()*(theApp.tower[t].header->index); 

}

/************************************************************************/
/*函数原型：void moves(int t1,int t2)									*/                                                                   
/*传入参数：塔t1,塔t2													*/                                                                   
/*返 回 值：无										   					*/                                                                   
/*函数功能：从塔t1移动一个圆盘至t2						 				*/                                                                   
/************************************************************************/
void moves(int t1,int t2){
 
	int speed; 

	if (!theApp.AutoRun)
		return;
	 
	detachNode(theApp.tower[t1].header,towerCenX(t1),towerCenY(t1));
	 
	//移到t1上端
	speed=-1;
	while (theApp.discFocus.y>HY-discHeight() && theApp.AutoRun)
	{ 
		theApp.discFocus.y+=speed; 
		SDL_Delay(theApp.timeElapse);
	}
	
	//移到t2上端 
	speed=towerCenX(t1)<towerCenX(t2)? 1 : -1;  
	while (theApp.discFocus.x!=towerCenX(t2) && theApp.AutoRun)
	{
		theApp.discFocus.x+=speed;
		SDL_Delay(theApp.timeElapse);
	} 

	//加到塔中
	if(theApp.AutoRun)
	{ 
		incMove();
		JoinNode(t2); 
		SDL_Delay(200);
	}
	else
		JoinNode(t1); 
	  
}
 

