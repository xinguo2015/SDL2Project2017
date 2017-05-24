
#include "Russia.h"

/************************************************************************/
/*函数原型：void onLButtonDown(void *pCtrl)								*/                                                                   
/*传入参数：指向某个按钮的指针											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理用户按下按钮事件						 				*/                                                                   
/************************************************************************/
void onLButtonDown(void *pCtrl){
	 
	if (pCtrl==&theApp.StartBtn && theApp.gamestate==NotStart) 
	{
		Mix_PlayChannel( -1, theApp.audio_btnDown, 0 ); 
		theApp.StartBtn.data=strDown;  
	} 

	else if (pCtrl==&theApp.StopBtn && theApp.gamestate!=NotStart)
	{
		Mix_PlayChannel( -1, theApp.audio_btnDown, 0 ); 
		if (theApp.gamestate==Activate)
			theApp.StopBtn.data=stpDown;
		else if (theApp.gamestate==Pause)
			theApp.StopBtn.data=rsmDown;
		else if (theApp.gamestate==Terminate)
			theApp.StopBtn.data=stpOver;
	}  
	else if (pCtrl==&theApp.QuitBtn) 
	{
		theApp.QuitBtn.data=qtDown; 
		Mix_PlayChannel( -1, theApp.audio_btnDown, 0 ); 
	}
		
}

/************************************************************************/
/*函数原型：void loseMouse(void* pCtrl)									*/                                                                   
/*传入参数：指向某个按钮的指针											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理鼠标移出按钮事件						 				*/                                                                   
/************************************************************************/
void loseMouse(void* pCtrl){
	 
	if (pCtrl==&theApp.StartBtn && theApp.gamestate==NotStart)
		theApp.StartBtn.data=strExpose;
	else if (pCtrl==&theApp.StopBtn)
	{	
		if (theApp.gamestate==Activate)
			theApp.StopBtn.data=stpExpose;
		if (theApp.gamestate==Pause)
			theApp.StopBtn.data=rsmExpose;
		if (theApp.gamestate==Terminate)
			theApp.StopBtn.data=retExpose;
	} 
	else if (pCtrl==&theApp.QuitBtn)
		theApp.QuitBtn.data=qtExpose; 
}

/************************************************************************/
/*函数原型：void onMouseMove(void *pCtrl)								*/                                                                   
/*传入参数：指向某个按钮的指针											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理鼠标经过按钮事件						 				*/                                                                   
/************************************************************************/
void onMouseMove(void *pCtrl){
	 
	void* pOld=getFocus();  
	if (pOld && pOld!=pCtrl)
		loseMouse(pOld);  
	if (pCtrl==&theApp.StartBtn && theApp.gamestate==NotStart)  
		theApp.StartBtn.data=strOver; 
	else if (pCtrl==&theApp.StopBtn)
	{ 
		if (theApp.gamestate==Activate)
			theApp.StopBtn.data=stpOver;
		else if (theApp.gamestate==Pause)
			theApp.StopBtn.data=rsmOver;
		else if (theApp.gamestate==Terminate)
			theApp.StopBtn.data=retOver;
	} 
	else if (pCtrl==&theApp.QuitBtn) 
		theApp.QuitBtn.data=qtOver; 
	setFocus(pCtrl);
}

/************************************************************************/
/*函数原型：void onLButtonUp(void *pCtrl)								*/                                                                   
/*传入参数：指向某个按钮的指针											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：处理用户释放按钮事件						 				*/                                                                   
/************************************************************************/
void onLButtonUp(void* pCtrl){ 

	SDL_QuitEvent ev; 
	 
	if (pCtrl==&theApp.StartBtn && theApp.gamestate==NotStart) 
	{
		theApp.StartBtn.data=strFix; 
		theApp.gamestate=Activate;
	} 
	else if (pCtrl==&theApp.StopBtn)
	{	
		if (theApp.gamestate==Activate)
		{
			theApp.StopBtn.data=rsmOver;
			theApp.gamestate=Pause;
		}
			
		else if (theApp.gamestate==Pause)
		{
			theApp.StopBtn.data=stpOver;
			theApp.gamestate=Activate;
		} 
		else if (theApp.gamestate==Terminate)
		{
			theApp.StopBtn.data=stpOver;
			theApp.gamestate=Activate;
			StartGame();
		} 
	} 
	else if (pCtrl==&theApp.QuitBtn)
	{
		theApp.QuitBtn.data=qtOver;
		ev.type=SDL_QUIT;
		SDL_PushEvent((SDL_Event *)&ev);
	} 
	setFocus(pCtrl);
}