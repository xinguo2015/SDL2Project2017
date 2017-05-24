#ifndef _CONSTANT_H_
#define _CONSTANT_H_ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>  
#include <SDL_thread.h>
 
#define MAXCOST 0x7fffffff
#define SCREEN_WIDTH 630	 
#define SCREEN_HEIGHT 660
#define ROW 10	//图的行数
#define COL 10	//图的列数 
#define TOTAL_TILES (2*ROW+1)*(2*COL+1)-ROW*COL*2+1//地图中的墙壁总数
#define MAX(a,b) a>b?a:b
#define MIN(a,b) a<b?a:b

#define BLANK 0  //标记地图中的空白位置
#define WALL  1	//地图中的墙	
#define STEP  2 //标记走过该位置

typedef int BOOL;
#define FALSE 0  
#define TRUE  1 

typedef enum { 
	btnExposed=0,			  
	btnDown				  
} BtnState;

typedef enum { 
	sNewGame = 0, 
	sAutoRun,    // 自动运行 
	sGameFail,   // 游戏失败
	sGameWin,	 // 游戏胜利
} GameState;	

typedef enum { // user defined event 
	evtStartGame=0,			// renew a game  
	evtStartTimer,			// start timing
	evtStopTimer,			// stop timing 
	evtAutoRun,				// auto run
	evtStopShow,			// stop auto run
	evtGameFail,			// the game is lost
	evtGameWin,				// win the game 
} UserEvent;

typedef struct
{
	SDL_Rect rect;
	int data; // data on the button
} ImageButton;

typedef struct {

	int mWidth;
	int mHeight;
	SDL_Texture* mTexture; 
}LTexture;

typedef struct{

	SDL_Rect rect; 
	int velx;
	int vely;

}Pumpkin;

typedef struct{

	SDL_Window *gWindow;
	SDL_Renderer *gRenderer;   
	TTF_Font *gFont; 
	LTexture btnLT;
	LTexture partLT; 
	int nbtnSprite;
	int npartSprite;
	LTexture levelLT;
	LTexture successLT[2]; //游戏胜利的提示 
	LTexture failLT[2];	//游戏失败的提示 
	LTexture numbersLT[10];//0-9数字纹理 
	int DigitW;	//个位数纹理宽度，取0-9纹理中最大值
	int DigitH;	//个位数纹理宽度，取0-9纹理中最大值
	 
	int MGraph[ROW*COL][ROW*COL];		//邻接矩阵，用于生成树，树代表迷宫中的所有通路
	int GameBoard[ROW*2+1][ROW*2+1];	//地图标志数组，gameboard[i][j]标志其为墙壁或者空地
	SDL_Rect tiles[TOTAL_TILES];		//墙壁的包围盒组
	SDL_Rect entryRect;
	SDL_Rect exitRect;

	SDL_TimerID timerID;	 
	ImageButton timeBtn;	//倒计时按钮	
	ImageButton autoBtn;	//自动演示按钮
	ImageButton manualBtn;	//手动操作按钮
	ImageButton upBtn;		//加速播放自动演示
	ImageButton downBtn;	//减速播放自动演示
	
	GameState gamestate;
	Pumpkin pumpkin;
	int level;//游戏难度等级 
	int Speed;
	BOOL autoRun;
	BOOL OnTheWay;	//标记此位置是否在通往出口的路径上		
	int timeElapse;	//自动演示时，南瓜每次移动的时间间隔
	void* pFocus;
	SDL_Thread* threadID;

}MineApp; 

extern MineApp theApp;
 
#endif
