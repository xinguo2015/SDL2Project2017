/****************************************************************** 
File name: Draw.c
Date: 2015/7/28
Description: 绘制类函数，绘制游戏中各个部件 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"


/************************************************************************/
/*函数原型：void drawNum(int x,int y,int num)							*/                                                                   
/*传入参数：x坐标，y坐标，数字num										*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：以（x,y）为顶部中心绘制数字num								*/                                                                   
/************************************************************************/
void drawNum(int x,int y,int num){
	 
	int i=0,width=0,len=0;
	int numbers[10];
	for (i=0;i<10;i++)
	{
		numbers[i]=-1;
	} 

	if (num==0)
	{
		numbers[0]=0;
		width+=theApp.numbersLT[0].mWidth;
	}
	i=0;
	while (num>0)
	{
		numbers[i]=num%10;//将num的各位数字存入numbers
		width+=theApp.numbersLT[numbers[i]].mWidth;//数字纹理宽度
		num/=10;
		i++;
	}
	for (i=9;i>-1;i--)
	{
		if (numbers[i]>=0)
		{
			renderTexture(theApp.numbersLT[numbers[i]],x-width/2+len,y);
			len+=theApp.numbersLT[numbers[i]].mWidth;
		}
	} 
}

/************************************************************************/
/*函数原型：void drawDisc(int x,int y,int index)						*/                                                                   
/*传入参数：x坐标，y坐标，圆盘标志index									*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：以（x,y）为底部中心点绘制index圆盘							*/                                                                   
/************************************************************************/
void drawDisc(int x,int y,int index){

	int sx,sy;
	SDL_Rect srcRect,destRect;
	sx=x-discW()/2;
	sy=y-discHeight();
	 
	MakeRect(&srcRect,0,discHeight()*index,discW(),discHeight());
	MakeRect(&destRect,sx,sy,discW(),discHeight());
	 
	SDL_RenderCopy(theApp.gRenderer,theApp.discsLT.mTexture,&srcRect,&destRect); 
}	

/************************************************************************/
/*函数原型：void drawDiscFocus()										*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制移动中的圆盘							 				*/                                                                   
/************************************************************************/
void drawDiscFocus(){
	 
	if (theApp.discFocus.node)
	{ 
		drawDisc(theApp.discFocus.x,theApp.discFocus.y+discHeight()/2,theApp.discFocus.node->index); 
	}
	
}	

/************************************************************************/
/*函数原型：void drawDiscInTower(int towerId,int disNum,int index)	*/                                                                   
/*传入参数：底座ID，下方圆盘数量，圆盘标志index							*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：在指定底座绘制圆盘							 				*/                                                                   
/************************************************************************/
void drawDiscInTower(int towerId,int disNum,int index){

	int x=HX+towerWidth()/2+(HSPACE+towerWidth())*towerId;
	int y=HY+towerHeight()-HW-discHeight()*disNum;

	drawDisc(x,y,index); 
}	

/************************************************************************/
/*函数原型：void DrawDiscs()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制所有圆盘								 				*/                                                                   
/************************************************************************/
void DrawDiscs(){

	LNode *node;
	int i,j=0;
	int disc=0;

	for (i=0;i<3;i++)
	{
		node=theApp.tower[i].header->next;
		disc=theApp.tower[i].header->index;
		while (node)
		{
			drawDiscInTower(i,disc-1-j,node->index);
			node=node->next;
			j++;
		}
		j=0;
	}
	drawDiscFocus();
}	

/************************************************************************/
/*函数原型：void DrawBackground()										*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏背景纹理							 				*/                                                                   
/************************************************************************/
void DrawBackground(){

	renderTexture(theApp.bgLT,0,0);
	
	//程序中直接指定每个底座的位置，与此相对应的直接指定底座的包围盒
	renderTexture(theApp.towerLT,HX,HY); 
	renderTexture(theApp.towerLT,HX+towerWidth()+HSPACE,HY); 
	renderTexture(theApp.towerLT,HX+(towerWidth()+HSPACE)*2,HY);	

	//书写移动次数
	drawNum(SCREEN_WIDTH/2,SCREEN_HEIGHT-30,theApp.move);
	//书写圆盘数量 
	drawNum(SCREEN_WIDTH/5+15,SCREEN_HEIGHT-30,theApp.discs);
	//书写成功提示 
	if (IsWinning())
	{
		renderTexture(theApp.winLT,SCREEN_WIDTH/2-theApp.winLT.mWidth/2,theApp.buttons[0].rect.y+btnHeight()+50);
	}
}	

/************************************************************************/
/*函数原型：void DrawButtons()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏按钮								 				*/                                                                   
/************************************************************************/
void DrawButtons(){

	int i,x,y;
	SDL_Rect srcRect;

	for (i=0;i<5;i++)
	{
		x=theApp.buttons[i].btnIndex*btnWidth();
		y=theApp.buttons[i].state*btnHeight();
		MakeRect(&srcRect,x,y,btnWidth(),btnHeight());

		SDL_RenderCopy(theApp.gRenderer,theApp.btnLT.mTexture,&srcRect,&theApp.buttons[i].rect);
		
	}
	 
}	

/************************************************************************/
/*函数原型：void renderApp()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏所有部件											*/                                                                   
/************************************************************************/
void renderApp(){

	SDL_RenderClear( theApp.gRenderer );

	DrawBackground();  

	DrawDiscs();

	DrawButtons();

	SDL_RenderPresent( theApp.gRenderer );

}