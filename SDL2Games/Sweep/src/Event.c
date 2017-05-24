/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "miner.h"

/************************************************************************/
/*函数原型：Uint32 onTimer(Uint32 interval, void *param)				*/                                                                   
/*传入参数：与前次调用的时间间隔interval，空类型指针param				*/                                                                   
/*返 回 值：下一次调用本函数的时间间隔									*/                                                                   
/*函数功能：SDL标准计时回调函数，每隔一秒游戏计时加一					*/                                                                   
/************************************************************************/ 
Uint32 onTimer(Uint32 interval, void *param)
{ 
	theApp.timeButton.data++; 
	if( theApp.timeButton.data>=999 )
	{ 
		SDL_RemoveTimer(theApp.timerID);
		theApp.timerID = NULL;
	}
	// continue to receive the timer events on the same time interval (1s)
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
/*函数功能：处理自定义的用户事件（如开始计时，开始游戏等）				*/                                                                   
/************************************************************************/
void onUserEvent( SDL_UserEvent *pEvent )
{
	// chck the user event code
	switch (pEvent->code)
	{ 
	case evtFaceOp:
		// change the face to 'o' image -> lbtn down
		theApp.faceButton.data = FaceOp; 
		break; 
	case evtFaceSmile:
		// change the face back to 'smile' -> lbtn up
		theApp.faceButton.data = FaceSmile; 
		break; 
	case evtMarkMore:
		// decrease the mine count
		theApp.taskButton.data--; 
		break;
	case evtMarkLess:
		// increase the mine count
		theApp.taskButton.data++; 
		break; 
	case evtStartTimer: 
		theApp.timerID = SDL_AddTimer(1000, onTimer, NULL);
		break;
		
	case evtNewGame:   
		Mix_PlayChannel( -1, theApp.audio_start, 0 ); 
		StartGame();
		break;
	case evtStopTimer: 
		SDL_RemoveTimer(theApp.timerID);
		theApp.timerID = NULL;
		break;
	case evtGameFail: 
	case evtGameWin:
		theApp.mineField.state = sGameOver; 
		theApp.faceButton.data = ( pEvent->code==evtGameWin ? FaceGlass : FaceFail ); 
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
	SDL_MouseButtonEvent* mb = (SDL_MouseButtonEvent*)event;
	SDL_MouseMotionEvent* mm = (SDL_MouseMotionEvent*)event;
	SDL_QuitEvent ev;
	void *pNow;
	int  key;

	switch( event->type )
	{ 
	case SDL_MOUSEBUTTONDOWN: // mouse button down event
		pNow = GetControlAt(mb->x, mb->y);
		if( !pNow ) break;
		SetFocus(pNow);
		if ( mb->button == 1 )
			onLButtonDown(pNow, mb->x, mb->y, mb->state);
		else
			onRButtonDown(pNow, mb->x, mb->y, mb->state);
		break;
		
	case SDL_MOUSEBUTTONUP: // mouse button down event
		pNow = GetControlAt(mb->x, mb->y);
		if( ! pNow || pNow!=GetFocus() ) break;
		if ( mb->button == 1 )
			onLButtonUp(pNow, mb->x, mb->y, mb->state); 
		else
			onRButtonUp(pNow, mb->x, mb->y, mb->state); 
		break; 

	case SDL_MOUSEMOTION: // mouse button down event
		pNow = GetControlAt(mm->x, mm->y);
		onMouseMove(pNow, mm->x, mm->y, mm->state);
		break; 

	case SDL_KEYDOWN:
		key = event->key.keysym.sym; 
		if( key==SDLK_ESCAPE )
		{
			ev.type = SDL_QUIT; 
			SDL_PushEvent( (SDL_Event*)&ev );
		}
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
				break;
				
			default:
				routeEvent(&event);
				break; 
			}
		}
		 
		if( MissionCompleted() )
		{ 
			if (theApp.winNotPlay)
			{
				theApp.winNotPlay=FALSE;
				Mix_PlayChannel( -1, theApp.audio_win, 0 ); 
			}
			theApp.winNotPlay=FALSE;
			postUserEvent(evtStopTimer);
			postUserEvent(evtGameWin);
		}  

		renderApp();
	} 
}