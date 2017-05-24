/****************************************************************** 
File name: Event.c
Date: 2015/8/7
Description: 事件处理类函数 
*******************************************************************/
#include "maze.h"

/************************************************************************/
/*函数原型：Uint32 onTimer(Uint32 interval, void *param)				*/                                                                   
/*传入参数：与前次调用的时间间隔interval，空类型指针param				*/                                                                   
/*返 回 值：下一次调用本函数的时间间隔									*/                                                                   
/*函数功能：SDL标准计时回调函数，每次运行倒计时减一						*/                                                                   
/************************************************************************/ 
Uint32 onTimer(Uint32 interval, void *param)
{ 

	theApp.timeBtn.data--; 
	if( theApp.timeBtn.data<=0 )
	{ 
		//倒计时结束，游戏失败
		postUserEvent(evtGameFail);
	}
	//以相同的时间间隔再次触发此计时回调函数
	return interval;
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
		//为游戏的逻辑变量赋值
		theApp.Speed=3;  
		setpumpkinVelX(0);//使南瓜速度为0
		setpumpkinVelY(0);
		theApp.timeBtn.data=199;  
		theApp.OnTheWay=FALSE;	
		theApp.timeElapse=32;
		theApp.gamestate=sNewGame;
		 //生成随机迷宫
		generateMap(ROW,COL);  
		postUserEvent(evtStartTimer);	
		break;
	//开始倒计时
	case evtStartTimer: 
		if (!theApp.timerID)
		{
			theApp.timerID = SDL_AddTimer(MAX(500-theApp.level*40,1), onTimer, NULL);
		} 
		break; 
	//暂停倒计时
	case evtStopTimer: 
		if (theApp.timerID)
		{
			SDL_RemoveTimer(theApp.timerID);
			theApp.timerID = NULL;  
		} 
		break;
	//开启自动演示
	case evtAutoRun:   
		if (!theApp.autoRun)
		{ 
			theApp.autoRun=TRUE;
			theApp.threadID = SDL_CreateThread( threadFunction, "maze", NULL ); 
			postUserEvent(evtStopTimer);
		} 
		break;  
	//关闭自动演示
	case evtStopShow:  
		if (theApp.autoRun)
		{
			theApp.autoRun=FALSE; 
			SDL_WaitThread(theApp.threadID,NULL);  
			resetGameBoard(ROW,COL);
		}  
		break;
	
	case evtGameWin:
		if (!GameOver())
		{
			postUserEvent(evtStopTimer);
			postUserEvent(evtStopShow);
			theApp.gamestate=sGameWin; 
		} 
		break; 

	case evtGameFail:
		if (!GameOver())
		{
			postUserEvent(evtStopTimer);
			postUserEvent(evtStopShow);
			theApp.gamestate=sGameFail;
		} 
		break;
	}
}

/************************************************************************/
/*函数原型：void onKeyDown(int key)										*/                                                                   
/*传入参数：键盘按键值													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理用户按下键盘事件										*/                                                                   
/************************************************************************/
void onKeyDown(int key){

	if (theApp.gamestate==sGameWin && key==SDLK_SPACE)
	{//闯关成功，难度增加
		theApp.level++;
		postUserEvent(evtStartGame);
	}
	else if (theApp.gamestate==sGameFail && key==SDLK_SPACE)
	{//游戏失败，重新开始
		theApp.level=0;
		postUserEvent(evtStartGame);
	}
	else if (!theApp.autoRun)
	{//游戏正在进行，控制南瓜移动
		switch(key){

		case SDLK_UP: 
			setpumpkinVelY(-theApp.Speed); 
			break;

		case SDLK_DOWN:
			setpumpkinVelY(theApp.Speed); 
			break;

		case SDLK_LEFT: 
			setpumpkinVelX(-theApp.Speed); 
			break;

		case SDLK_RIGHT: 
			setpumpkinVelX(theApp.Speed); 
			break; 
		}  
	} 
}

/************************************************************************/
/*函数原型：void onKeyUp(int key)										*/                                                                   
/*传入参数：键盘按键值													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理用户释放方向键事件										*/                                                                   
/************************************************************************/
void onKeyUp(int key){
	
	if (GameOver() || theApp.autoRun) 
		return; 
	
	if (key== SDLK_UP || key==SDLK_DOWN)
	{ 
		setpumpkinVelY(0);
	}
	else if (key=SDLK_LEFT || key==SDLK_RIGHT)
	{ 
		setpumpkinVelX(0);
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
	int key;
	void *pNow;
	SDL_QuitEvent ev;  
	SDL_MouseButtonEvent* mb = (SDL_MouseButtonEvent*)event;
	SDL_MouseMotionEvent* mm = (SDL_MouseMotionEvent*)event;

	switch( event->type )
	{   
	case SDL_KEYDOWN:  
		key = event->key.keysym.sym; 
		if( key==SDLK_ESCAPE )	{ 
			ev.type = SDL_QUIT; 
			SDL_PushEvent( (SDL_Event*)&ev ); 
		}   
		else  
			onKeyDown(key); 
		break;   
	case SDL_KEYUP:   
		key = event->key.keysym.sym;  
		onKeyUp(key);
		break;  
	case SDL_MOUSEBUTTONDOWN:  
		pNow = GetControlAt(mb->x, mb->y);
		if( !pNow ) break;
		SetFocus(pNow);
		if ( mb->button == 1 )
			onLButtonDown(pNow); 
		break;   
	case SDL_MOUSEBUTTONUP:    
		pNow = GetControlAt(mm->x, mm->y);
		onLButtonUp(pNow);
		break; 
	case SDL_MOUSEMOTION:  
		pNow = GetControlAt(mm->x, mm->y);
		onMouseMove(pNow);
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
	BOOL quit = FALSE;
	SDL_Event event;

	while( !quit)
	{ 
		//处理事件
		while( SDL_PollEvent(&event) )
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = TRUE;  
				postUserEvent(evtStopShow);
				break;

			default: 
				routeEvent(&event);
				break; 
			}
		}

		//更新南瓜的位置
		if (!theApp.autoRun)
		{
			updatePumpkin();
		}  
		//绘制游戏界面
		renderApp();	
	}   
}