/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/

#include "miner.h"
 
/**************************************************************	*****************/
/*函数原型：void onLButtonDown(void *pCtrl, int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：焦点部件的指针，X坐标，Y坐标，备用参数flags							*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标左键按下事件												*/                                                                   
/********************************************************************************/
void onLButtonDown(void *pCtrl, int x, int y, unsigned char flags)
{  
	if( pCtrl==&theApp.mineField && ! GameOver() )
	{
		postUserEvent(evtFaceOp);
		LeftPressField(x,y,flags);
	}
	else if( pCtrl==&theApp.faceButton ) 
		theApp.faceButton.data = FaceSmileSink;  
} 

/**************************************************************	*****************/
/*函数原型：void onLButtonUp(void *pCtrl, int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：焦点部件的指针，X坐标，Y坐标，备用参数flags							*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标左键释放事件												*/                                                                   
/********************************************************************************/
void onLButtonUp(void *pCtrl, int x, int y, unsigned char flags)
{
	if( pCtrl==&theApp.faceButton )
	{
		postUserEvent(evtFaceSmile);
		postUserEvent(evtNewGame);
	}
	else if( pCtrl==&theApp.mineField  && ! GameOver() )
	{
		postUserEvent(evtFaceSmile);
		LeftReleaseField(x,y,flags);
	}
	SetFocus(NULL);
} 

/**************************************************************	*****************/
/*函数原型：void onRButtonDown(void *pCtrl, int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：焦点部件的指针，X坐标，Y坐标，备用参数flags							*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标右键按下事件												*/                                                                   
/********************************************************************************/
void onRButtonDown(void *pCtrl, int x, int y, unsigned char flags)
{
	if( pCtrl==&theApp.mineField  && ! GameOver() )
	{
		RightPressField(x,y,flags);
	}
}

/**************************************************************	*****************/
/*函数原型：void onRButtonUp(void *pCtrl, int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：焦点部件的指针，X坐标，Y坐标，备用参数flags							*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标右键释放事件												*/                                                                   
/********************************************************************************/
void onRButtonUp(void *pCtrl, int x, int y, unsigned char flags)
{
	if( pCtrl==&theApp.mineField  && ! GameOver() )
	{
		postUserEvent(evtFaceSmile);
		RightReleaseField(x,y,flags);
	}	
}

/**************************************************************	*****************/
/*函数原型：void LoseMouse(void * pCtrl)										*/                                                                   
/*传入参数：焦点部件的指针														*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标移出卡通脸按钮事件											*/                                                                   
/********************************************************************************/
void LoseMouse(void * pCtrl)
{
	if( pCtrl==&theApp.faceButton )
	{
		if( theApp.faceButton.data!=FaceSmile )
			postUserEvent(evtFaceSmile);
	}
}

/**************************************************************	*****************/
/*函数原型：void onMouseMove(void *pCtrl, int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：焦点部件的指针，X坐标，Y坐标，备用参数flags							*/                                                                   
/*返 回 值：无  																*/                                                                   
/*函数功能：处理鼠标移动事件													*/                                                                   
/********************************************************************************/
void onMouseMove(void * pCtrl, int x, int y, unsigned char flags)
{
	void * pOld = GetFocus();

	if( pOld && pOld != pCtrl )
		LoseMouse(pOld);

	if ( ! pOld ) return; // no control to handle

	if( pOld==&theApp.mineField  && ! GameOver() )
		MoveOnField(x,y,flags);
}

