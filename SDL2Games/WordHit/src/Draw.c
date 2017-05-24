/****************************************************************** 
File name: Draw.c
Date: 2015/7/17
Description: 绘制类函数，绘制游戏中各个部件 
*******************************************************************/
#include "hitword.h"

/************************************************************************/
/*函数原型：void DrawBackground()										*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制游戏背景纹理							 				*/                                                                   
/************************************************************************/
void DrawBackground(){

	SDL_Rect srcRect;
	  
	MakeRect( &srcRect,0,theApp.background.scrollingOffset,theApp.SCREEN_WIDTH,theApp.SCREEN_HEIGHT);

	SDL_RenderCopy( theApp.gRenderer, theApp.background.gback.mTexture, &srcRect, NULL ); 
	 
}	

/************************************************************************/
/*函数原型：void DrawWords()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制所有单词										 		*/                                                                   
/************************************************************************/
void DrawWords(){

	int i;   
	for (i=0;i<theApp.screen_words;i++) 
		draw_one_word(i); 
} 

/************************************************************************/
/*函数原型：void DrawBombs()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制所有炮弹										 		*/                                                                   
/************************************************************************/
void DrawBombs(){

	int i;
	SDL_Rect destRect;
	for (i=0;i<theApp.screen_bombs;i++)
	{  
		MakeRect(&destRect,bombX(i),bombY(i),theApp.gbomb.mWidth,theApp.gbomb.mHeight); 
		
		SDL_RenderCopyEx(theApp.gRenderer,theApp.gbomb.mTexture,NULL,&destRect,bombAngle(i),NULL,SDL_FLIP_NONE);
	}

}

/************************************************************************/
/*函数原型：void DrawBooms()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制所有爆炸序列										 	*/                                                                   
/************************************************************************/
void DrawBooms(){

	int i,x,w,h;  
	SDL_Rect srcRect;
	 
	for (i=0;i<10;i++)
	{
		if (theApp.expls[i].valid)
		{
			w=theApp.expls[i].rect.w;
			h=theApp.expls[i].rect.h;
			x=w*theApp.expls[i].frame;
			MakeRect(&srcRect,x,0,w,h); 
			SDL_RenderCopy(theApp.gRenderer,theApp.gexplosion.mTexture,&srcRect,&theApp.expls[i].rect);
			
			//更新爆炸帧，判断是否已经播放完毕
			if (++theApp.expls[i].frame>=theApp.nExpSprite)
			{
				theApp.expls[i].valid=FALSE;
				theApp.expls[i].frame=-1;
			}
		}
		
	}

}

/************************************************************************/
/*函数原型：void DrawBlood()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制UFO血条									 				*/                                                                   
/************************************************************************/
void DrawBlood(){
	SDL_Rect srcRect,destRect;
	
	//绘制血条背景
	renderTexture(theApp.blood.gbloodBg,10,10);
	
	//绘制血条值
	if (UFOScreenHP()>UFOHP())
	{
		theApp.blood.screen_hp-=1;
	} 
	MakeRect(&srcRect,0,0,HPWidth()*theApp.blood.screen_hp/100,HPHeight());
	MakeRect(&destRect,10,10,HPWidth()*theApp.blood.screen_hp/100,HPHeight());

	SDL_RenderCopy(theApp.gRenderer,theApp.blood.gblood.mTexture,&srcRect,&destRect); 
}

/************************************************************************/
/*函数原型：void DrawUFO()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制UFO										 				*/                                                                   
/************************************************************************/
void DrawUFO(){
	    
	renderTexture(theApp.ufo.gufo,UFOPosX(),UFOPosY());
}

/************************************************************************/
/*函数原型：void DrawFail()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：绘制失败界面										 		*/                                                                   
/************************************************************************/
void DrawFail(){
	if (!theApp.gameover)
		return ; 
	renderTexture(theApp.gGameOver1,theApp.SCREEN_WIDTH/2-theApp.gGameOver1.mWidth/2,theApp.SCREEN_HEIGHT/4);
	renderTexture(theApp.gGameOver2,theApp.SCREEN_WIDTH/2-theApp.gGameOver2.mWidth/2,theApp.SCREEN_HEIGHT/4+theApp.gGameOver1.mHeight*2);
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
	DrawWords();
	DrawBombs();
	DrawBlood();
	DrawUFO(); 
	DrawBooms();  
	DrawFail();
		

	SDL_RenderPresent( theApp.gRenderer );

}
