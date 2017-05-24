/****************************************************************** 
File name: LNode.c
Date: 2015/8/11
Description:鼠标事件处理函数 
*******************************************************************/
#include "maze.h"

/************************************************************************/
/*函数原型：void scale_downBtn(ImageButton* pCtrl)						*/                                                                   
/*传入参数：按钮指针pCtrl												*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：按下按钮，该按钮形状缩小到90%								*/                                                                   
/************************************************************************/
void scale_downBtn(ImageButton* pCtrl){

	int x,y,w,h; 
	x=pCtrl->rect.x;
	y=pCtrl->rect.y;
	w=pCtrl->rect.w;
	h=pCtrl->rect.h;

	//按钮形状缩小到90%	
	if (pCtrl->data==btnExposed)
	{
		x=x+w/18;
		y=y+h/18;
		w=w*9/10;
		h=h*9/10; 
		MakeRect(&pCtrl->rect,x,y,w,h); 

		pCtrl->data=btnDown;
	} 
}

/************************************************************************/
/*函数原型：void scale_upBtn(ImageButton* pCtrl)						*/                                                                   
/*传入参数：按钮指针pCtrl												*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：使按钮形状恢复正常											*/                                                                   
/************************************************************************/
void scale_upBtn(ImageButton* pCtrl){

	int x,y,w,h;  
	x=pCtrl->rect.x;
	y=pCtrl->rect.y;
	w=pCtrl->rect.w;
	h=pCtrl->rect.h;

	//按钮形状恢复正常
	if (pCtrl->data==btnDown)
	{
		x=x-w/18;
		y=y-h/18;
		w=w*10/9;
		h=h*10/9; 
		MakeRect(&pCtrl->rect,x,y,w,h); 

		pCtrl->data=btnExposed;
	} 
}

/************************************************************************/
/*函数原型：void releaseBtn(ImageButton* pCtrl)							*/                                                                   
/*传入参数：按钮指针pCtrl												*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：释放按钮，该按钮形状恢复正常								*/                                                                   
/************************************************************************/
void releaseBtn(ImageButton* pCtrl){
	
	//钮形状恢复正常
	scale_upBtn(pCtrl);
	//如果释放的是自动演示按钮
	if (pCtrl==&theApp.autoBtn)
	{ 
		postUserEvent(evtAutoRun); 
	}
	//如果释放的是手动操作按钮	
	if (pCtrl==&theApp.manualBtn)
	{
		theApp.autoRun=FALSE;  
		postUserEvent(evtStartTimer);
	}
	//如果释放的是加速播放按钮
	if (pCtrl==&theApp.upBtn)
	{
		theApp.timeElapse=MAX(theApp.timeElapse/2,4);
	}
	//如果释放的是减速播放按钮
	if (pCtrl==&theApp.downBtn)
	{
		theApp.timeElapse=MIN(theApp.timeElapse*2,1024);
	}
}

/************************************************************************/
/*函数原型：void onLButtonDown(void* pCtrl)								*/                                                                   
/*传入参数：空类型指针pCtrl												*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标左键按下按钮事件									*/                                                                   
/************************************************************************/
void onLButtonDown(void* pCtrl)
{   
	if (!GameOver())
	{
		scale_downBtn((ImageButton *)pCtrl); 
	}
	
} 
 
/************************************************************************/
/*函数原型：void onLButtonUp(void* pCtrl)								*/                                                                   
/*传入参数：空类型指针pCtrl												*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标左键释放事件										*/                                                                   
/************************************************************************/
void onLButtonUp(void* pCtrl)
{     
	if( ! pCtrl ||GameOver())  
		return;
	//如果pCtrl指向按钮 
	releaseBtn((ImageButton *)pCtrl);
} 

/************************************************************************/
/*函数原型：void onMouseMove(void* pCtrl)								*/                                                                   
/*传入参数：空类型指针pCtrl												*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标移动事件											*/                                                                   
/************************************************************************/
void onMouseMove(void * pCtrl)
{ 
	void * pOld; 
	if (GameOver())
		return; 
	pOld = GetFocus();
	//如果鼠标移出按钮
	if (pOld && (!pCtrl))
	{   
		scale_upBtn((ImageButton*) pOld);
		SetFocus(NULL);
	}   
}