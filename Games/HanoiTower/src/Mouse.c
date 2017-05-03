/****************************************************************** 
File name: Mouse.c
Date: 2015/7/31
Description:鼠标处理函数 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"
 

/**************************************************************	*****************/
/*函数原型：void onLButtonDown(void* pCtrl,int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：焦点部件pCtrl,X坐标，Y坐标，备用参数flags							*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标左键按下事件												*/                                                                   
/********************************************************************************/
void onLButtonDown(void* pCtrl,int x, int y, unsigned char flags)
{   
	if (pCtrl>=(void*)(theApp.buttons) && pCtrl<=(void*)(theApp.buttons+4))
		pressBtn((Button*)pCtrl); 
	else if (!theApp.AutoRun)
	{
		detachNode((LNode*)pCtrl,x,y); 
	} 
} 

/**************************************************************	*****************/
/*函数原型：void onLButtonUp(int x, int y, unsigned char flags)					*/                                                                   
/*传入参数：X坐标，Y坐标，备用参数flags											*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标左键释放事件												*/                                                                   
/********************************************************************************/
void onLButtonUp(int x, int y, unsigned char flags)
{   
	void* pNow;
	if (IsDraging())
	{
		RejoinNode(x,y); 
	} 
	else
	{
		pNow = GetControlAt(x, y);
		if( ! pNow || pNow!=GetFocus() )  
			return;
		releaseBtn((Button*)pNow);
	}

} 

/**************************************************************	*****************/
/*函数原型：void onMouseMove(void * pCtrl,int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：X坐标，Y坐标，备用参数flags											*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标移动事件													*/                                                                   
/********************************************************************************/
void onMouseMove(void * pCtrl,int x, int y, unsigned char flags)
{ 
	void * pOld = GetFocus();
	  
	if (IsDraging())
	{
		theApp.discFocus.x=x;
		theApp.discFocus.y=y;  
	}
	else if (pOld && (!pCtrl))
	{  
		LoseMouse((Button*) pOld);
	}
	 
}
 
/**************************************************************	*****************/
/*函数原型：void LoseMouse(void * pCtrl)										*/                                                                   
/*传入参数：焦点部件的指针														*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标移出按钮事件												*/                                                                   
/********************************************************************************/
void LoseMouse(Button* pCtrl)
{
	if(pCtrl>=theApp.buttons && pCtrl<=theApp.buttons+4)
	{
		pCtrl->state=btnExposed;	 
	}
}
