/****************************************************************** 
File name: Draw.c
Date: 2015/8/7
Description: 绘制类函数 
*******************************************************************/
#include "maze.h"

/************************************************************************/
/*函数原型：void drawNum(int x,int y,int num)							*/                                                                   
/*传入参数：x坐标，y坐标，数字num(0-999)								*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：以（x,y）为中心绘制数字										*/                                                                   
/************************************************************************/
void drawNum(int x,int y,int num){

	int i=0,len=0,width=0;
	int numbers[3]={0};

	for (i=0;i<3;i++)
	{
		numbers[i]=-1;
	} 
	//将num的各位数字存入numbers数组，并计算num组合纹理的总长度
	if (num==0) 
	{
		numbers[0]=0;
		width=theApp.DigitW; 
	}
		
	i=0;
	while (num>0)
	{
		numbers[i]=num%10;
		width+=theApp.DigitW;//每位数字大小一致，避免倒计时的抖动现象
		num/=10;
		i++;
	} 
	//逐位绘制数字
	for (i=2;i>=0;i--)
	{ 
		if (numbers[i]>=0)
		{
			renderTexture(theApp.numbersLT[numbers[i]],x-width/2+len,y-theApp.DigitH/2);
			len+=theApp.DigitW; 
		}
	} 
}

/************************************************************************/
/*函数原型：void drawBtn(ImageButton* btn)								*/                                                                   
/*传入参数：按钮指针													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制按钮btn													*/                                                                   
/************************************************************************/
void drawBtn(ImageButton* btn){

	int x=0,y;
	int w=btnW(),h=btnH();

	SDL_Rect srcRect;
	//根据按钮选择相应的纹理
	if (btn==&theApp.autoBtn)
		y=0;//自动播放纹理
	else if (btn==&theApp.manualBtn)
		y=btnH();//手动操作纹理
	else if (btn==&theApp.timeBtn)
		y=btnH()*2; //计时器背景纹理
	else if (btn==&theApp.upBtn)
	{
		//加速播放纹理 
		y=btnH()*3;
		w=btnW()/2;
	}
	else if (btn==&theApp.downBtn)
	{
		//减速播放纹理
		x=btnW()/2;
		y=btnH()*3; 
		w=btnW()/2;
	} 

	MakeRect(&srcRect,x,y,w,h); 
	SDL_RenderCopy(theApp.gRenderer,theApp.btnLT.mTexture,&srcRect,&btn->rect); 
	
	if (btn==&theApp.timeBtn)
	{//绘制计时数字
		drawNum(SCREEN_WIDTH/2,partH()/2+3,theApp.timeBtn.data);
	}
		
}

/************************************************************************/
/*函数原型：void drawLevel()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏难度等级											*/                                                                   
/************************************************************************/
void drawLevel(){

	 renderTexture(theApp.levelLT,10,partH()/2-theApp.levelLT.mHeight/2);
	 drawNum(theApp.levelLT.mWidth+15+theApp.DigitW,partH()/2+2,theApp.level+1);
}
  
/************************************************************************/
/*函数原型：void DrawWall()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏墙壁								 				*/                                                                   
/************************************************************************/
void DrawWall(){

	int i,j,x,y; 
	//绘制墙壁
	for (i=0;i<2*ROW+1;i++)
	{
		for(j=0;j<2*COL+1;j++)
		{
			if (theApp.GameBoard[i][j]==WALL)
			{  
				x=j*partW();
				y=(i+1)*partH(); 
				renderPart(theApp.partLT,0,theApp.npartSprite,x,y);
			}
		}
	}
}
 
/************************************************************************/
/*函数原型：void DrawToolBar()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制屏幕顶端工具栏								 			*/                                                                   
/************************************************************************/
void DrawToolBar(){

	//绘制工具栏纯色背景
	SDL_Rect fillRect={0,0,SCREEN_WIDTH,partH()};
	SDL_SetRenderDrawColor(theApp.gRenderer,29,90,111,0xFF); 
	SDL_RenderFillRect(theApp.gRenderer,&fillRect);
	//绘制难度等级
	drawLevel();
	//绘制计时器
	 drawBtn(&theApp.timeBtn);
	//绘制按钮
	 drawBtn(&theApp.manualBtn);
	 if (theApp.autoRun)
	 { 
		 //如果当前处于自动演示状态，则显示加速减速按钮
		 drawBtn(&theApp.upBtn);
		 drawBtn(&theApp.downBtn);
	 }
	 else
	 {
		 //否则显示自动演示按钮
		 drawBtn(&theApp.autoBtn);
	 }
	 
}

/************************************************************************/
/*函数原型：void DrawDoors()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏出入口								 				*/                                                                   
/************************************************************************/
void DrawDoors(){
	
	SDL_Rect srcRect={0,partH(),partW(),partH()};
	//绘制入口
	SDL_RenderCopy(theApp.gRenderer,theApp.partLT.mTexture,&srcRect,&theApp.entryRect);
	//绘制出口
	SDL_RenderCopy(theApp.gRenderer,theApp.partLT.mTexture,&srcRect,&theApp.exitRect); 
}

/************************************************************************/
/*函数原型：void DrawPumpkin()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制南瓜									 				*/                                                                   
/************************************************************************/
void DrawPumpkin(){
	
	SDL_Rect srcRect={0,partH()*2,partW(),partH()};

	SDL_RenderCopy(theApp.gRenderer,theApp.partLT.mTexture,&srcRect,&theApp.pumpkin.rect);
}

/************************************************************************/
/*函数原型：void DrawTips()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：如果游戏结束，绘制提示										*/                                                                   
/************************************************************************/
void DrawTips(){

	if(theApp.gamestate==sGameWin)
	{
		//如果游戏成功，绘制成功提示 
		renderTexture(theApp.successLT[0],SCREEN_WIDTH/2-theApp.successLT[0].mWidth/2,SCREEN_HEIGHT/4);
		renderTexture(theApp.successLT[1],SCREEN_WIDTH/2-theApp.successLT[1].mWidth/2,SCREEN_HEIGHT/4+theApp.successLT[0].mHeight*2);
	
	}
	else if (theApp.gamestate==sGameFail)
	{ //如果游戏失败，绘制失败提示 
		renderTexture(theApp.failLT[0],SCREEN_WIDTH/2-theApp.failLT[0].mWidth/2,SCREEN_HEIGHT/4);
		renderTexture(theApp.failLT[1],SCREEN_WIDTH/2-theApp.failLT[1].mWidth/2,SCREEN_HEIGHT/4+theApp.failLT[0].mHeight*2); 
	}
}

/************************************************************************/
/*函数原型：void renderApp()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏所有部件											*/                                                                   
/************************************************************************/
void renderApp(){

	SDL_SetRenderDrawColor(theApp.gRenderer,16,38,52,255);
	SDL_RenderClear( theApp.gRenderer );

	//绘制墙壁
	DrawWall();
	//绘制工具栏
	DrawToolBar();
	//绘制出入口
	DrawDoors();
	//绘制南瓜
	DrawPumpkin();
	//绘制提示
	DrawTips();
	
	SDL_RenderPresent( theApp.gRenderer );

}



