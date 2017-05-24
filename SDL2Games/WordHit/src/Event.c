/****************************************************************** 
File name: Event.c
Date: 2015/7/17
Description: 事件处理类函数 
*******************************************************************/
#include "hitword.h"
 
/************************************************************************/
/*函数原型：void startGame()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：每局游戏开始前初始化数据									*/                                                                   
/************************************************************************/ 
void startGame(){ 

	//清空单词数组
	clearWords();
	theApp.timeout=SDL_GetTicks()+2000;
	theApp.hit=-1;//当前拼写出单词
	theApp.hitlen=0; 
	theApp.screen_words=0;		
	
	theApp.gameover=FALSE;
	theApp.background.scrollingOffset=0;
	theApp.blood.hp=100;//血量
	theApp.blood.screen_hp=100;//屏幕显示的血量 
	//清空炮弹数组
	clearBombs(); 
	//播放背景音乐
	if( Mix_PlayingMusic() == 0 ) 
		Mix_PlayMusic( theApp.gMusic, -1 ); 

}

/************************************************************************/
/*函数原型：void updateApp()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：更新游戏数据												*/                                                                   
/************************************************************************/ 
void updateApp()
{   
	if (theApp.gameover)
		return ;  

	//更新背景纹理裁剪位置
	theApp.background.scrollingOffset--;
	if (theApp.background.scrollingOffset<=0)
	{
		theApp.background.scrollingOffset=theApp.background.gback.mHeight-theApp.SCREEN_HEIGHT;
	} 

	//定时产生单词
	if (SDL_TICKS_PASSED(SDL_GetTicks(), theApp.timeout))
	{
		generate_one_word();
		theApp.timeout=SDL_GetTicks()+2000; 
	}
	//更新炮弹信息  
	updateBombs();

	//更新单词信息  
	updateWords();
	 
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

		case evtStopMusic:
			if( Mix_PausedMusic() == 1 ) 
				Mix_ResumeMusic(); 
			else 
				Mix_PauseMusic(); 
			break;
	}
}
  
/************************************************************************/
/*函数原型：void onKeyDown(int key)										*/                                                                   
/*传入参数：键盘按键值													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理用户打字事件											*/                                                                   
/************************************************************************/
void onKeyDown(int key){
	
	int i; 
	if(theApp.hit > -1) // if the word was once hit
	{
		 
		if(theApp.word[theApp.hit].word[theApp.hitlen] == key) 
		{ 
			theApp.hitlen++; 
			if(theApp.hitlen == strlen(theApp.word[theApp.hit].word))
			{ 
				fire_a_Bomb(theApp.hit); 
				theApp.hit = -1;      // 当前无打中的单词
				theApp.hitlen = 0;   // 打中的字母长度=0
			}
		}
		else // wrong key entered
		{  
			theApp.hit = -1;
			theApp.hitlen = 0;
		}
	}
	else // if the word is hit for the 1st time
	{
		for(i=0; i<theApp.screen_words; i++)
		{
			if(theApp.word[i].word[0] == key) 
				break; 
		}
		if(i<theApp.screen_words)
		{ 
			theApp.hit = i;
			theApp.hitlen = 1; 
			if(theApp.hitlen == strlen(theApp.word[theApp.hit].word))
			{
				fire_a_Bomb(theApp.hit); // 显示子弹 
				theApp.hit = -1; 
				theApp.hitlen = 0;
			}
		}  
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
	SDL_QuitEvent ev; 
	int  key; 
	switch( event->type )
	{  
	case SDL_KEYDOWN:
		key = event->key.keysym.sym; 
		if( key==SDLK_ESCAPE )	
		{//退出游戏
			ev.type = SDL_QUIT; 
			SDL_PushEvent( (SDL_Event*)&ev );
			break; 
		} 
		else if( key==SDLK_SPACE && theApp.gameover)
		{//重新开始游戏
			postUserEvent(evtStartGame); 
			break; 
		}
		else if (key==SDLK_F1)
		{//暂停音乐
			postUserEvent(evtStopMusic); 
			break; 
		}
		else if (!theApp.gameover) 
		{//拼写单词
			onKeyDown(key);
			break; 
		} 	 
	case SDL_WINDOWEVENT:
		//拖拽窗口时，不生成新的单词
		if (event->window.event==SDL_WINDOWEVENT_MOVED) 
			theApp.timeout = SDL_GetTicks() + 2000; 
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
			//
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
		//更新游戏数据
		updateApp(); 
		//绘制游戏界面
		renderApp();  
	} 
}


