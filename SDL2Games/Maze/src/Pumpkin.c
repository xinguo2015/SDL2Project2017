/****************************************************************** 
File name: Util.c
Date: 2015/8/8
Description: 工具类函数 
*******************************************************************/
#include "maze.h"

  
/************************************************************************/
/*函数原型：int pumpkinX()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：南瓜的X坐标					 								*/                                                                   
/*函数功能：获取南瓜的X坐标												*/                                                                   
/************************************************************************/
int pumpkinX(){

	return theApp.pumpkin.rect.x;
} 

/************************************************************************/
/*函数原型：int pumpkinY()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：南瓜的Y坐标													*/                                                                   
/*函数功能：获取南瓜的Y坐标												*/                                                                   
/************************************************************************/
int pumpkinY(){

	return theApp.pumpkin.rect.y;
} 

/************************************************************************/
/*函数原型：void incPumpkinX(int x)										*/                                                                   
/*传入参数：移动距离													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定南瓜在水平方向上的移动量								*/                                                                   
/************************************************************************/
void incPumpkinX(int x){

	theApp.pumpkin.rect.x+=x;
}

/************************************************************************/
/*函数原型：void incPumpkinY(int y)										*/                                                                   
/*传入参数：移动距离													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定南瓜在竖直方向上的移动量								*/                                                                   
/************************************************************************/
void incPumpkinY(int y){

	theApp.pumpkin.rect.y+=y;
}
 
/************************************************************************/
/*函数原型：int pumpkinVelX()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：南瓜水平方向的移动速度										*/                                                                   
/*函数功能：获取南瓜水平方向的移动速度									*/                                                                   
/************************************************************************/
int pumpkinVelX(){

	return theApp.pumpkin.velx;
}

/************************************************************************/
/*函数原型：void setpumpkinVelX(int velx)								*/                                                                   
/*传入参数：南瓜水平方向的移动速度										*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定南瓜水平方向的移动速度									*/                                                                   
/************************************************************************/
void setpumpkinVelX(int velx){

	theApp.pumpkin.velx=velx; 
}

/************************************************************************/
/*函数原型：int pumpkinVelY()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：南瓜竖直方向的移动速度										*/                                                                   
/*函数功能：获取南瓜竖直方向的移动速度									*/                                                                   
/************************************************************************/
int pumpkinVelY(){

	return theApp.pumpkin.vely; 
}

/************************************************************************/
/*函数原型：void setpumpkinVelY(int vely)								*/                                                                   
/*传入参数：南瓜竖直方向的移动速度										*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定南瓜竖直方向的移动速度									*/                                                                   
/************************************************************************/
void setpumpkinVelY(int vely){

	theApp.pumpkin.vely=vely;   
}

/************************************************************************/
/*函数原型：int verticalTest(int x,int y,int velx)						*/                                                                   
/*传入参数：X坐标，Y坐标，水平方向的速度								*/                                                                   
/*返 回 值：竖直方向上的移动量											*/                                                                   
/*函数功能：当南瓜试图向右转弯或者向左转弯，计算其在竖直方向是否需要移动*/                                                                   
/************************************************************************/
int verticalTest(int x,int y,int velx){

	int i,j;
	i=y/partW()-1;//坐标（x,y）对应GameBoard[][]数组的下标,屏幕顶端有一行提示栏，所以-1
	j=x/partH();
	//如果（x,y）所处区域的正右方是BLANK且南瓜试图向右移动，则其往屏幕上方移动
	if (theApp.GameBoard[i][j+1]==BLANK && velx>0)
		return -(MAX(theApp.Speed/2,1));
	//如果（x,y）所处区域的正左方是BLANK且南瓜试图向左移动，则其往屏幕上方移动
	else if (theApp.GameBoard[i][j-1]==BLANK && velx<0)
		return -(MAX(theApp.Speed/2,1));
	//如果（x,y）所处区域的右下方是BLANK且南瓜试图向右移动，则其往屏幕下方移动
	else if(theApp.GameBoard[i+1][j+1]==BLANK && velx>0) 
		return MAX(theApp.Speed/2,1);
	//如果（x,y）所处区域的左下方是BLANK且南瓜试图向左移动，则其往屏幕下方移动
	else if(theApp.GameBoard[i+1][j-1]==BLANK && velx<0) 
		return MAX(theApp.Speed/2,1);
	else
		return 0;
}	

/************************************************************************/
/*函数原型：int horizontalTest(int x,int y,int vely)					*/                                                                   
/*传入参数：X坐标，Y坐标，竖直方向的速度								*/                                                                   
/*返 回 值：水平方向上的移动量											*/                                                                   
/*函数功能：当南瓜试图向上转弯或者向下转弯，计算其在水平方向是否需要移动*/                                                                   
/************************************************************************/
int horizontalTest(int x,int y,int vely){

	int i,j;
	i=y/partW()-1;//坐标（x,y）对应GameBoard[][]数组的下标,屏幕顶端有一行提示栏，所以-1
	j=x/partH(); 
	//如果（x,y）所处区域的正上方是BLANK且南瓜试图向上移动，则其往屏幕左方移动
	if (theApp.GameBoard[i-1][j]==BLANK && vely<0) 
		return -(MAX(theApp.Speed/2,1)); 
	//如果（x,y）所处区域的正下方是BLANK且南瓜试图向下移动，则其往屏幕左方移动
	else if (theApp.GameBoard[i+1][j]==BLANK && vely>0)  
		return -(MAX(theApp.Speed/2,1)); 
	//如果（x,y）所处区域的右上方是BLANK且南瓜试图向上移动，则其往屏幕右方移动
	else if(theApp.GameBoard[i-1][j+1]==BLANK && vely<0)  
		return MAX(theApp.Speed/2,1); 
	//如果（x,y）所处区域的右下方是BLANK且南瓜试图向下移动，则其往屏幕右方移动
	else if(theApp.GameBoard[i+1][j+1]==BLANK && vely>0) 
		return MAX(theApp.Speed/2,1);
	return 0;
}	

/************************************************************************/
/*函数原型：void updatePumpkin()										*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：更新南瓜的位置												*/                                                                   
/************************************************************************/
void updatePumpkin(){
	
	if (GameOver())
		return;
	incPumpkinX(pumpkinVelX());
	//在水平方向移动时发生碰撞则回退
	if (touchesWall( theApp.pumpkin.rect, theApp.tiles, TOTAL_TILES))
	{   
		incPumpkinX(-pumpkinVelX());
		if (pumpkinVelY()==0)
		{ //在转角位置滑动转向
			incPumpkinY(verticalTest(pumpkinX(),pumpkinY(),pumpkinVelX()));
		}
	}

	incPumpkinY(pumpkinVelY());
	//在竖直方向移动时发生碰撞则回退
	if (touchesWall( theApp.pumpkin.rect, theApp.tiles, TOTAL_TILES))
	{    
		incPumpkinY(-pumpkinVelY());
		if (pumpkinVelX()==0)  
		{//在转角位置滑动转向 
			incPumpkinX(horizontalTest(pumpkinX(),pumpkinY(),pumpkinVelY())); 
		} 
	}
	//判断游戏是否成功
	if (IsSucceed())
	{
		postUserEvent(evtGameWin);
	}
}
 
/************************************************************************/
/*函数原型：void moveUp(int destY)										*/                                                                   
/*传入参数：目标位置Y坐标												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：使南瓜竖直向上移动至destY									*/                                                                   
/************************************************************************/
void moveUp(int destY){  
	if (!theApp.autoRun)
		return;   
	//设定南瓜的速度
	setpumpkinVelY(-theApp.Speed); 
	//南瓜持续向上移动
	while (pumpkinY()>destY && theApp.autoRun)
	{ 
		//如果南瓜与目标位置的距离小于速度的大小，则改变速度
		if (pumpkinY()+pumpkinVelY()<destY)
		{
			setpumpkinVelY(destY-pumpkinY()); 
		}
		//更新南瓜的位置
		updatePumpkin();  
		SDL_Delay(theApp.timeElapse); 
	}
	setpumpkinVelY(0);
}

/************************************************************************/
/*函数原型：void moveDown(int destY)									*/                                                                   
/*传入参数：目标位置Y坐标												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：使南瓜竖直向下移动至destY									*/                                                                   
/************************************************************************/
void moveDown(int destY){  
	if (!theApp.autoRun)
		return;  
	//设定南瓜的速度
	setpumpkinVelY(theApp.Speed); 
	//南瓜持续向下移动
	while (pumpkinY()<destY && theApp.autoRun)
	{ 
		//如果南瓜与目标位置的距离小于速度的大小，则改变速度
		if (pumpkinY()+pumpkinVelY()>destY)
		{
			setpumpkinVelY(pumpkinY()-destY);
		}
		//更新南瓜的位置	
		updatePumpkin();  
		SDL_Delay(theApp.timeElapse);  
	}
	setpumpkinVelY(0);
}

/************************************************************************/
/*函数原型：void moveLeft(int destX)									*/                                                                   
/*传入参数：目标位置X坐标												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：使南瓜水平向左移动至destX									*/                                                                   
/************************************************************************/
void moveLeft(int destX){  
	if (!theApp.autoRun)
		return; 
	//设定南瓜的速度
	setpumpkinVelX(-theApp.Speed); 
	//南瓜持续向左移动
	while (pumpkinX()>destX && theApp.autoRun)
	{ 
		//如果南瓜与目标位置的距离小于速度的大小，则改变速度
		if (pumpkinX()+pumpkinVelX()<destX)
		{
			setpumpkinVelX(destX-pumpkinX());
		}
		//更新南瓜的位置
		updatePumpkin();   
		SDL_Delay(theApp.timeElapse); 
	}
	setpumpkinVelX(0);
}

/************************************************************************/
/*函数原型：void moveRight(int destX)									*/                                                                   
/*传入参数：目标位置X坐标												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：使南瓜水平向右移动至destX									*/                                                                   
/************************************************************************/
void moveRight(int destX){ 
	if (!theApp.autoRun)
		return; 
	//设定南瓜的速度
	setpumpkinVelX(theApp.Speed); 
	//南瓜持续向右移动
	while (pumpkinX()<destX && theApp.autoRun)
	{ 
		//如果南瓜与目标位置的距离小于速度的大小，则改变速度
		if (pumpkinX()+pumpkinVelX()>destX)
		{
			setpumpkinVelX(pumpkinX()-destX);
		} 
		//更新南瓜的位置
		updatePumpkin(); 
		SDL_Delay(theApp.timeElapse); 
	}
	setpumpkinVelX(0);
}