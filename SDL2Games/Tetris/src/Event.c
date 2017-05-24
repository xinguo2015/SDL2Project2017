#include "Russia.h"


/************************************************************************/
/*函数原型：void handleUp()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：当玩家按了UP键时变换图形						 			*/                                                                   
/************************************************************************/ 
void handleUp(){
	 
	int NextBlockId,i;
	static int arrayOffset[5]={0,-1,1,-2,2};
	 
	NextBlockId = arrayBLOCK[getCurrentBlockID()].nNext; 
	for (i=0;i<5;i++)
	{
		if (!IsConflict(NextBlockId,getShapeX()+arrayOffset[i],getShapeY()))
		{
			setCurrentBlockID(NextBlockId);
			alterShapeX(arrayOffset[i]);
			break;
		}
	} 
} 

/************************************************************************/
/*函数原型：void handleLeft()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：当玩家按了LEFT键时尝试左移图形						 		*/                                                                   
/************************************************************************/  
void handleLeft(){

	if (!IsConflict(getCurrentBlockID(),getShapeX()-1,getShapeY()))
			alterShapeX(-1);
}

/************************************************************************/
/*函数原型：void handleRight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：当玩家按了RIGHT键时尝试左移图形						 		*/                                                                   
/************************************************************************/  
void handleRight(){

	if (!IsConflict(getCurrentBlockID(),getShapeX()+1,getShapeY()))
		alterShapeX(1);
}

/************************************************************************/
/*函数原型：void handleDown()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理形状下落的行数									 		*/                                                                   
/************************************************************************/  
void handleDown(){
	SDL_Event ev; 
	int x,y,i,BlockIndex;
	int NumLinesKilled;		
	x=getShapeX();
	y=getShapeY();
	BlockIndex=getCurrentBlockID();
	if (!IsConflict(BlockIndex,x,y+1))
	{  
		alterShapeY(1); 
		return; 
	}
	else 
	{ 
		Mix_PlayChannel( -1, theApp.audio_attach, 0 ); 
		//修改Gameboard
		for (i=0;i<=7;i+=2)
		{
			if (y+arrayBLOCK[BlockIndex].arrXY[i+1]<1) 
				continue;  
			theApp.Gameboard[x+arrayBLOCK[BlockIndex].arrXY[i]][y+arrayBLOCK[BlockIndex].arrXY[i+1]]=arrayBLOCK[BlockIndex].boxtype; 
		}  
		//计算并修改可消去的行
		NumLinesKilled=KillLines(y); 
		
		//根据消去的行数更新得分,调整难度
		if (NumLinesKilled>0)
		{
			Mix_PlayChannel( -1, theApp.audio_score, 0 );

			switch (NumLinesKilled)
			{
			case 1: 
				incScore(100);
				break;
			case 2:
				incScore(300);
				break;
			case 3:
				incScore(500);
				break;
			case 4:
				incScore(800);
				break;
			}
		}
		nextShape();
		 
		if (IsScoreUp())
		{
			Mix_PlayChannel( -1, theApp.audio_levelUp, 0 ); 
			incLevel(); 
		} 
		if (IsGameOver())
		{
			Mix_PlayChannel( -1, theApp.audio_fail, 0 );
			theApp.gamestate=Terminate;
			setFocus(&theApp.StopBtn); 
  			ev.type=SDL_MOUSEMOTION;
			SDL_PushEvent(&ev); 
		} 
	}
	
}

/************************************************************************/
/*函数原型：void onKeyDown(int key) 									*/                                                                   
/*传入参数：键盘键值													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理键盘事件									 			*/                                                                   
/************************************************************************/
void onKeyDown(int key){

	SDL_QuitEvent ev; 
	
	if (key==SDLK_ESCAPE)
	{
		ev.type=SDL_QUIT;
		SDL_PushEvent((SDL_Event *)&ev);
	}
	//如果游戏处于进行状态
	if (theApp.gamestate==Activate)
	{
		switch (key)
		{ 
		case SDLK_UP: 
			handleUp();	
			break;
		case SDLK_LEFT: 
			handleLeft();	
			break;
		case SDLK_RIGHT: 
			handleRight();	
			break;
		case SDLK_DOWN: 
			setAccel(TRUE);
			break;
		}
	}
	 
}
 
/************************************************************************/
/*函数原型：void handleEvents(SDL_Event *event)							*/                                                                   
/*传入参数：SDL_Event													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：根据事件类型，向其他函数传递任务参数			 			*/                                                                   
/************************************************************************/
void handleEvents(SDL_Event *event){
 
	SDL_MouseButtonEvent* mb=(SDL_MouseButtonEvent*) event;
	SDL_MouseMotionEvent* mm=(SDL_MouseMotionEvent*) event;
	void* pNow;

	switch (event->type)
	{
	case SDL_KEYDOWN:
			onKeyDown(event->key.keysym.sym);
		break; 
	case SDL_KEYUP:
		if (event->key.keysym.sym==SDLK_DOWN)
			setAccel(FALSE);
		break; 
	case SDL_MOUSEMOTION:
		pNow=getControlAt(mb->x,mb->y); 
		onMouseMove(pNow);
		break;
	case SDL_MOUSEBUTTONDOWN: 
		if (mb->button==1)
		{
			pNow=getControlAt(mb->x,mb->y); 
			if(!pNow) break;
			setFocus(pNow); 
			onLButtonDown(pNow);
		} 
		break;
	case SDL_MOUSEBUTTONUP:
		if (mb->button==1)
		{
			pNow=getControlAt(mb->x,mb->y);
			if(!pNow || pNow!=getFocus()) break; 
			onLButtonUp(pNow);
		}
		break; 
	} 
}
 
/************************************************************************/
/*函数原型：void RunApp()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：游戏主框架，接受玩家触发的事件 ，绘制界面  					*/                                                                   
/************************************************************************/
void RunApp(){

	BOOL quit=FALSE;
	SDL_Event event;

	StartGame(); 

	while (!quit)
	{ 
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit=TRUE;
				break;
			default:
				handleEvents(&event);
				break;
			}
		} 
		if (theApp.gamestate==Activate && (IsAccel()|| IsTimeUp(400-50*getLevel()))) 
			handleDown();
		renderApp();  
		SDL_Delay(50);
	}

}






