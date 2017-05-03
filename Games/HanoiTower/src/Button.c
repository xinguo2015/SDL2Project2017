/****************************************************************** 
File name: Button.c
Date:	2015/8/4
Description:	按钮相关函数 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"
 
/************************************************************************/
/*函数原型：int btnWidth()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：获取单个按钮的宽度											*/                                                                   
/************************************************************************/
int btnWidth(){

	return theApp.btnLT.mWidth/theApp.btnCol;
}

/************************************************************************/
/*函数原型：int btnHeight()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：获取单个按钮的高度											*/                                                                   
/************************************************************************/
int btnHeight(){

	return theApp.btnLT.mHeight/theApp.btnRow;
}

/************************************************************************/
/*函数原型：void pressBtn(Button* pCtrl)								*/                                                                   
/*传入参数：被按下按钮的指针											*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：按下按钮处理函数											*/                                                                   
/************************************************************************/
void pressBtn(Button* pCtrl){
	 
	pCtrl->state=btnDown;  
}

/************************************************************************/
/*函数原型：void handleAdd()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：处理加号按钮事件											*/                                                                   
/************************************************************************/
void handleAdd()
{
	if (IsBeginning())
	{ 
		incDiscs();
		postUserEvent(evtStartGame);
	}
	else if (theApp.AutoRun && theApp.timeElapse>1)
	{
		theApp.timeElapse--;
	} 
}

/************************************************************************/
/*函数原型：void handleMinus()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：处理减号按钮事件											*/                                                                   
/************************************************************************/
void handleMinus()
{
	if (IsBeginning())
	{ 
		decDiscs();
		postUserEvent(evtStartGame);
	}
	else if (theApp.AutoRun && theApp.timeElapse<10)
	{
		theApp.timeElapse++;
	} 
}

/************************************************************************/
/*函数原型：void releaseBtn(Button* pCtrl)								*/                                                                   
/*传入参数：释放按钮的指针												*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：释放按钮处理函数											*/                                                                   
/************************************************************************/
void releaseBtn(Button* pCtrl){

	LoseMouse(pCtrl);

	if (pCtrl==&theApp.buttons[0])
		handleAdd();

	else if (pCtrl==&theApp.buttons[1]) 
		handleMinus(); 

	else if (pCtrl==&theApp.buttons[2] && IsBeginning()) 
		postUserEvent(evtAutoRun);  
	 
	else if (pCtrl==&theApp.buttons[3]) 
		theApp.AutoRun=FALSE;  

	else if (pCtrl==&theApp.buttons[4])
		postUserEvent(evtStartGame);
}