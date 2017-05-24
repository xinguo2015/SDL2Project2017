#include "Russia.h"


/************************************************************************/
/*函数原型：void drawBackground()										*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏背景纹理							 				*/                                                                   
/************************************************************************/
void drawBackground(){
	
	SDL_RenderCopy( theApp.gRenderer, theApp.imgBackground.mTexture, NULL, NULL ); 
}
 
/************************************************************************/
/*函数原型：void drawBox(int x,int y,BoxType boxtype)  					*/                                                                   
/*传入参数：游戏中的绝对坐标(x,y),待绘制方块的类型						*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：在指定位置绘制一个指定类型的方块			 				*/                                                                   
/************************************************************************/
void drawBox(int x,int y,BoxType boxtype){

	SDL_Rect srcRect;
	SDL_Rect destRect;

	if (y<1) return;

	srcRect.x=0;
	srcRect.y=BoxHeight()*(boxtype-1);
	srcRect.w=BoxWidth();
	srcRect.h=BoxHeight();

	destRect.x=BOXSIZE*x;
	destRect.y=BOXSIZE*y;
	destRect.w=BoxWidth();
	destRect.h=BoxHeight();

	SDL_RenderCopy(theApp.gRenderer,theApp.imgBox.mTexture,&srcRect,&destRect);
}
   
/************************************************************************/
/*函数原型：void drawBoxes()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制已落入面板，且未被消除的方块			 				*/                                                                   
/************************************************************************/
void drawBoxes(){

	int i,j;
	for (j=1;j<=BOARD_HEIGHT;j++)
		for (i=1;i<=BOARD_WIDTH;i++)
		{
			drawBox(i,j,theApp.Gameboard[i][j]); 
		}

}

/************************************************************************/
/*函数原型：void drawBtn()												*/                                                                   
/*传入参数：指向按钮的指针															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制指定的按钮								 				*/                                                                   
/************************************************************************/
void drawBtn(ImageButton* btn){

	 SDL_Rect srcRect;
	 
	 srcRect.x=0;
	 srcRect.y=ButtonHeight()*btn->data;
	 srcRect.w=ButtonWidth();
	 srcRect.h=ButtonHeight();

	 SDL_RenderCopy(theApp.gRenderer,btn->ltexture.mTexture,&srcRect,&btn->rect); 
}

/************************************************************************/
/*函数原型：void drawButtons()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏中的开始，暂停，退出按钮			 				*/                                                                   
/************************************************************************/
void drawButtons(){

	drawBtn(&theApp.StartBtn);
	drawBtn(&theApp.StopBtn);
	drawBtn(&theApp.QuitBtn);

}
 
/************************************************************************/
/*函数原型：void drawBlock(int BlockIndex,int sx,int sy)				*/                                                                   
/*传入参数：待绘制图形的索引，全局坐标（sx,sy）							*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：在全局坐标（sx，sy）处绘制BlockIndex类型的形状 				*/                                                                   
/************************************************************************/
void drawBlock(int BlockIndex,int sx,int sy){
	int i;
	for (i=0;i<7;i+=2)  
		drawBox(arrayBLOCK[BlockIndex].arrXY[i]+sx,arrayBLOCK[BlockIndex].arrXY[i+1]+sy,arrayBLOCK[BlockIndex].boxtype); 

}

/************************************************************************/
/*函数原型：void drawShape()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏中正下落的图形和下个图形提示		 				*/                                                                   
/************************************************************************/
void drawShape(){
	 
	int x,y,blockindex;
	if (theApp.gamestate==NotStart || theApp.gamestate==Terminate)
		return;

	x=theApp.shape.X;
	y=theApp.shape.Y;
	blockindex=theApp.shape.nCurrent_block_index;
	drawBlock(blockindex,x,y); 

	x=25; 
	y=12;
	blockindex=theApp.shape.nNext_block_index; 
	drawBlock(blockindex,x,y); 

}

/************************************************************************/
/*函数原型：void drawLevel()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制当前游戏难度等级	 					 				*/                                                                   
/************************************************************************/
void drawLevel(){

	SDL_RenderCopy(theApp.gRenderer,theApp.Level.ltexture.mTexture,NULL,&theApp.Level.rect);
}

/************************************************************************/
/*函数原型：void drawScore()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制当前得分			 					 				*/                                                                   
/************************************************************************/
void drawScore(){
	 
	SDL_RenderCopy(theApp.gRenderer,theApp.Score.ltexture.mTexture,NULL,&theApp.Score.rect); 
}

/************************************************************************/
/*函数原型：void drawFailBg()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制失败纹理			 					 				*/                                                                   
/************************************************************************/
void drawFailBg(){

	SDL_Rect rect={30,50,theApp.imgFail.mWidth,theApp.imgFail.mHeight};
	SDL_RenderCopy( theApp.gRenderer, theApp.imgFail.mTexture, NULL, &rect );
}

/************************************************************************/
/*函数原型：void renderApp()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏背景，按钮，正下落图形，未消除方块，计分和等级		*/                                                                   
/************************************************************************/
void renderApp(){
	 
	SDL_RenderClear( theApp.gRenderer );
	
	drawBackground();
	drawButtons();
	drawShape();
	drawBoxes();
	drawScore();
	drawLevel();
	if (theApp.gamestate==Terminate)
	{
		drawFailBg();
	}

	SDL_RenderPresent( theApp.gRenderer );

}


