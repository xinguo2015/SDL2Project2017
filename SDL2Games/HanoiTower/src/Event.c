/****************************************************************** 
File name: Event.c
Date: 2015/7/28
Description: 事件处理类函数 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"

 
/************************************************************************/
/*函数原型：void startGame()											*/                                                                   
/*传入参数：圆盘数量													*/                                                                   
/*返 回 值：无															*/
/*函数功能：程序开始前初始化部分数据									*/                                                                   
/************************************************************************/
void startGame(){

	int i;
	LNode *node;  
	if (theApp.AutoRun)
	{
		theApp.AutoRun=FALSE;
		SDL_WaitThread(theApp.threadID,NULL); 
	} 
	theApp.timeElapse=5;
	setMove(0);
	clearLists();
	for (i=0;i<theApp.discs;i++)
	{
		node=MakeNode(theApp.discs-1-i);
		AddNode(theApp.tower[0].header,node);
	} 
}
 
/************************************************************************/
/*函数原型：void postUserEvent(int code)								*/                                                                   
/*传入参数：事件代码 code												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：向SDL事件队列中推送自定义事件								*/                                                                   
/************************************************************************/
void postUserEvent(int code)
{ 
	SDL_UserEvent ev;
	ev.type = SDL_USEREVENT;
	ev.code = code;
	ev.data1 = NULL;
	ev.data2 = NULL; 

	SDL_PushEvent((SDL_Event*)&ev);
}

/************************************************************************/
/*函数原型：void onUserEvent( SDL_UserEvent *pEvent )					*/                                                                   
/*传入参数：SDL_UserEvent												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理自定义的用户事件										*/                                                                   
/************************************************************************/
void onUserEvent( SDL_UserEvent *pEvent ){
	 
	switch (pEvent->code){  
	case evtStartGame:  
		startGame();
		break;
	case evtAutoRun:  
		theApp.AutoRun=TRUE;
		theApp.threadID = SDL_CreateThread( threadFunction, "hanoi", NULL );
		break;  
	}
}

/************************************************************************/
/*函数原型：void routeEvent(SDL_Event* event)							*/                                                                   
/*传入参数：SDL_Event													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：核心函数，用于处理在游戏中的各种事件（如按下各种按键）		*/                                                                   
/************************************************************************/
void routeEvent(SDL_Event* event)
{ 
	void *pNow;
	SDL_QuitEvent ev;  
	SDL_MouseButtonEvent* mb = (SDL_MouseButtonEvent*)event;
	SDL_MouseMotionEvent* mm = (SDL_MouseMotionEvent*)event;
	 
	switch( event->type )
	{  
	case SDL_KEYDOWN: 
		if( event->key.keysym.sym==SDLK_ESCAPE )	
		{ 
			ev.type = SDL_QUIT; 
			SDL_PushEvent( (SDL_Event*)&ev );
			break; 
		}  
	case SDL_MOUSEBUTTONDOWN:  
		pNow = GetControlAt(mb->x, mb->y);
		if( !pNow ) break;
		SetFocus(pNow);
		if ( mb->button == 1 )
			onLButtonDown(pNow,mb->x, mb->y, mb->state); 
		break; 
	case SDL_MOUSEBUTTONUP:    
		if ( mb->button == 1 )
			onLButtonUp(mb->x, mb->y, mb->state);  
		break;  
	case SDL_MOUSEMOTION:  
		pNow = GetControlAt(mm->x, mm->y);
		onMouseMove(pNow,mm->x, mm->y, mm->state);
		break;  
	default:
		if( event->type >= SDL_USEREVENT && event->type < SDL_LASTEVENT )
			onUserEvent((SDL_UserEvent*)event);
		break;
	}
}

/************************************************************************/
/*函数原型：void RunApp()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：游戏主框架，接受玩家触发的事件 ，绘制界面  					*/                                                                   
/************************************************************************/
void RunApp()
{
	BOOL quit = 0;
	SDL_Event event;
	 
	while( !quit)
	{ 
		while( SDL_PollEvent(&event) )
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = TRUE; 
				if (theApp.AutoRun)
				{
					theApp.AutoRun=FALSE; 
					SDL_WaitThread(theApp.threadID,NULL); 
				} 
				break;

			default: 
				routeEvent(&event);
				break; 
			}
		}   
		renderApp();  
	}   
}
