/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#ifndef _CONSTANT_H_
#define _CONSTANT_H_ 
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>  
#include <SDL_thread.h>

#define SCREEN_WIDTH 800	 
#define SCREEN_HEIGHT 600	 
#define HW 20		//底座竖棒的宽度
#define HX 100		//第一个底座的位置
#define HY 260
#define HSPACE	60 //底座间距

typedef int BOOL;
#define FALSE 0  
#define TRUE  1   

typedef enum { 
	btnExposed=0,			  
	btnDown				  
} BtnState;

typedef enum { 
	btnAdd=0,			  
	btnMinus,
	btnPlay,
	btnHand,
	btnRefresh
} BtnIndex;
 
typedef enum { // user defined event 
	evtStartGame=1,			// renew a game  
	evtAutoRun 		 
} UserEvent;

typedef struct {

	int mWidth;
	int mHeight;
	SDL_Texture* mTexture; 

}LTexture;

typedef struct lnode{

	int index;	//0-8标志9种颜色与Rect
	struct lnode * next; 

}LNode;

typedef struct{ 

	LNode* header;	//头结点的index表示链表结点数
	SDL_Rect rect;	//本塔竖杆的包围盒 
}Tower;

typedef struct{

	BtnState state;
	BtnIndex btnIndex;
	SDL_Rect rect; 
}Button;

typedef struct{
	 
	int x;	
	int y;	//移动圆盘的中心点		
	LNode* h;	//记录圆盘被移动前的头结点
	LNode* node;

}DiscFocus;

typedef struct{

	SDL_Window *gWindow;
	SDL_Renderer *gRenderer;   
	TTF_Font *gFont;  
	SDL_Thread* threadID;

	LTexture bgLT;  
	LTexture towerLT;
	LTexture discsLT;
	LTexture numbersLT[10]; 
	LTexture btnLT;
	LTexture winLT;
	int ndiscSprite;
	int btnRow;
	int btnCol;

	Button buttons[5];
	Tower tower[3];
	DiscFocus discFocus;
	int discs;
	int move;
	int timeElapse;
	void* pFocus;
	BOOL AutoRun;

}MineApp; 
 
extern MineApp theApp;

 
#endif
 