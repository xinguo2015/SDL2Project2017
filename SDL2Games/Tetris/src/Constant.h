#ifndef _CONSTANT_H_
#define _CONSTANT_H_ 
#define BOXSIZE 20		/**每个方块占20个像素**/
#define BOARD_WIDTH 12	/**游戏面板的宽度，以一个方块的宽度为单位**/
#define BOARD_HEIGHT 22	/**游戏面板的高度，以一个方块的宽度为单位**/
#define SCREEN_WIDTH 640	/**游戏屏幕的宽度，以一个像素为单位**/
#define SCREEN_HEIGHT 480	/**游戏屏幕的高度，以一个像素为单位**/
 
typedef int BOOL;
#define FALSE 0  
#define TRUE  1   

typedef enum{
	NotStart,
	Activate,
	Pause,
	Terminate
}GameState;

typedef enum{	 
	strExpose=0,		//常规显示按钮
	strOver,		//鼠标经过按钮
	strDown,		//按钮已被按下
	strFix,			//游戏已经开始，按钮不可操作 
}StartBtnState;

typedef enum{	 
	stpExpose=0,		//显示暂停	 
	stpOver,		 
	stpDown,	
	rsmExpose,		//显示继续
	rsmOver,		 
	rsmDown,
	retExpose,		//显示重试
	retOver
}StopBtnState;

typedef enum{	 
	qtExpose=0,		//常规显示退出 
	qtOver,			//鼠标经过退出
	qtDown			//按钮已被按下 
}QuitBtnState;
 
typedef enum{ 
	BOX_BLANK=0,	/*空方块（表示当前位置没有方块）*/
	BOX_BLUE,	 	
	BOX_CYAN,
	BOX_GREEN,
	BOX_PINK,
	BOX_RED,
	BOX_YELLOW,
	BOX_ORANGE
} BoxType;

typedef struct {
	int arrXY[8];
	BoxType boxtype;
	int nNext;
} Block;

typedef struct  
{ 
	int nCurrent_block_index;     
	int nNext_block_index;  
	BOOL bAccel;
	int X;
	int Y; 
}Shape; 

typedef struct {

	SDL_Texture* mTexture; 
	int mWidth;
	int mHeight;
}LTexture;

typedef struct  {
	SDL_Rect rect;//dstRect
	int data;
	LTexture ltexture;
}ImageButton;
 
typedef struct{
	
	SDL_Window *gWindow;
	SDL_Renderer *gRenderer;   
	TTF_Font *gFont;  
	Uint32 time;

	LTexture imgBackground; 
	LTexture imgFail; 
	LTexture imgBox;
	Shape shape;
	ImageButton StartBtn;
	ImageButton StopBtn;
	ImageButton QuitBtn;
	ImageButton Score;
	ImageButton Level;

	int nStartBtnSprite;
	int nStopBtnSprite;
	int nQuitBtnSprite;
	int nBoxSprite;

	Mix_Chunk *audio_attach;
	Mix_Chunk *audio_levelUp;
	Mix_Chunk *audio_btnDown;
	Mix_Chunk *audio_score;
	Mix_Chunk *audio_fail;
	
	void* pFocus;
	GameState gamestate;
	BoxType Gameboard[BOARD_WIDTH+2][BOARD_HEIGHT+2];
	

}MineApp; 

extern MineApp theApp;
 
extern Block arrayBLOCK[19];/**游戏中总共有19种不同的形状**/
 
#endif