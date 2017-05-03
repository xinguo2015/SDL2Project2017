/****************************************************************** 
File name: Disc.c
Date: 2015/8/1
Descriptio: 圆盘操作类函数 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"


/************************************************************************/
/*函数原型：int discW()													*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：圆盘纹理的宽度										   		*/                                                                   
/*函数功能：获取圆盘纹理的宽度											*/                                                                   
/************************************************************************/
int discW(){

	return theApp.discsLT.mWidth;
}

/************************************************************************/
/*函数原型：int discHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：单个圆盘纹理的高度										   	*/                                                                   
/*函数功能：获取单个圆盘纹理的高度										*/                                                                   
/************************************************************************/
int discHeight(){

	return theApp.discsLT.mHeight/theApp.ndiscSprite;
}

/************************************************************************/ 
/*函数原型：int discWidth(int index)									*/                                                                   
/*传入参数：纹理标志index												*/                                                                   
/*返 回 值：圆盘纹理的可视宽度									   		*/                                                                   
/*函数功能：获取指定类型圆盘纹理的可视宽度								*/                                                                   
/************************************************************************/
int discWidth(int index){
	 
	return discW()-HW*(8-index);
}
  
/************************************************************************/
/*函数原型：void incDiscs()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：圆盘总数加一												*/                                                                   
/************************************************************************/
void incDiscs(){
	
	if (theApp.discs>8)
		return;
	theApp.discs++;  
}

/************************************************************************/
/*函数原型：void decDiscs()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：圆盘总数减一												*/                                                                   
/************************************************************************/
void decDiscs(){

	if (theApp.discs<2)
		return;
	theApp.discs--;
}

/************************************************************************/
/*函数原型：void setDiscs(int discs)									*/                                                                   
/*传入参数：圆盘数量discs												*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：设定程序中圆盘总数											*/                                                                   
/************************************************************************/
void setDiscs(int discs){

	theApp.discs=discs; 
}

/************************************************************************/
/*函数原型：void SetMove(int move)										*/                                                                   
/*传入参数：移动次数move												*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：设定程序中移动次数											*/                                                                   
/************************************************************************/
void setMove(int move){

	theApp.move=move; 
}

/************************************************************************/
/*函数原型：void incMove()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：移动次数数加一												*/                                                                   
/************************************************************************/
void incMove(){

	theApp.move++; 
}

/************************************************************************/
/*函数原型：int towerWidth()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：底座纹理的宽度										   		*/                                                                   
/*函数功能：获取底座纹理的宽度											*/                                                                   
/************************************************************************/
int towerWidth(){

	return theApp.towerLT.mWidth;
}

/************************************************************************/
/*函数原型：int towerHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：底座纹理的高度										   		*/                                                                   
/*函数功能：获取底座纹理的高度											*/                                                                   
/************************************************************************/
int towerHeight(){

	return theApp.towerLT.mHeight;
}

/************************************************************************/
/*函数原型：BOOL IsDraging()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：移动的圆盘不为空返回TRUE,否则返回FALSE				   		*/                                                                   
/*函数功能：判断鼠标是否正拖拽圆盘										*/                                                                   
/************************************************************************/
BOOL IsDraging(){
	 
	return theApp.discFocus.node!=NULL && (!theApp.AutoRun);
}
  
/************************************************************************/
/*函数原型：void* HitDiscTest(int x,int y)								*/                                                                   
/*传入参数：X坐标，Y坐标												*/                                                                   
/*返 回 值：坐标位于每列第一个圆盘内返回该列头结点,否则返回NULL			*/                                                                   
/*函数功能：判断坐标（x，y）是否选中某列第一个圆盘						*/                                                                   
/************************************************************************/
void* HitDiscTest(int x,int y){

	int i,sx,sy,discNum,discIndex;
	LNode* header;
	SDL_Rect rect; 
	void* pFocus=NULL;

	for (i=0;i<3;i++)
	{
		header=theApp.tower[i].header;
		discNum=header->index; 
		
		if (discNum>0)
		{ 
			discIndex=header->next->index;
			//计算每列第一个圆盘的rect
			sx=theApp.tower[i].rect.x+HW/2-discWidth(discIndex)/2;
			sy=HY+towerHeight()-HW-discHeight()*discNum; 
			MakeRect(&rect,sx,sy,discWidth(discIndex),discHeight());
			
			if (HitRectTest(x,y,rect))
			{
				pFocus=theApp.tower[i].header;
				break;
			} 
		}
	}
	return pFocus;
}
  
/************************************************************************/
/*函数原型：BOOL IsConflict(SDL_Rect rect1,SDL_Rect rect2)				*/                                                                   
/*传入参数：包围盒rect1,包围盒rect2										*/                                                                   
/*返 回 值：发生碰撞返回TRUE,否则返回FALSE						   		*/                                                                   
/*函数功能：AABB包围盒碰撞检测											*/                                                                   
/************************************************************************/
BOOL IsConflict(SDL_Rect rect1,SDL_Rect rect2){

	if (rect1.x+rect1.w<rect2.x)
		return FALSE;
	if (rect1.x>rect2.x+rect2.w)
		return FALSE;
	if (rect1.y+rect1.h<rect2.y)
		return FALSE;
	if (rect1.y>rect2.y+rect2.h)
		return FALSE;

	return TRUE;

}

/************************************************************************/
/*函数原型：BOOL IsDiscConflict(int x,int y,int index,SDL_Rect rect)	*/                                                                   
/*传入参数：坐标x,坐标y,圆盘标记index,包围盒rect						*/                                                                   
/*返 回 值：中心点位于（x,y）index类的圆盘碰撞rect返回TRUE,否则返回FALSE*/                                                                   
/*函数功能：圆盘的碰撞检测												*/                                                                   
/************************************************************************/
BOOL IsDiscConflict(int x,int y,int index,SDL_Rect rect){

	int sx,sy;
	SDL_Rect rect1;
	   
	sx=x-discWidth(index)/2;
	sy=y-discHeight()/2; 
	MakeRect(&rect1,sx,sy,discWidth(index),discHeight()); 

	return IsConflict(rect1,rect);
}

/************************************************************************/
/*函数原型：LNode* FindTouchedTower(int x,int y,int index)				*/                                                                   
/*传入参数：坐标x,坐标y,圆盘标记index									*/                                                                   
/*返 回 值：圆盘触碰底座且底座中的圆盘半径均更大，返回头结点，否则返回空*/                                                                   
/*函数功能：获取圆盘触碰且满足条件的底座头结点							*/                                                                   
/************************************************************************/
LNode* FindTouchedTower(int x,int y,int index){
	
	int i;
	LNode* header;
	Tower tower; 
	for (i=0;i<3;i++)
	{
		tower=theApp.tower[i];
		header=tower.header;
		if (IsDiscConflict(x,y,index,tower.rect))
		{ 
			if (!header->next || index<header->next->index)
			{
				return header;
			} 
		}
	}
	 
	return NULL;
}