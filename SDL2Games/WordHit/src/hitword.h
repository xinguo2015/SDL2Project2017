#ifndef __HITWORD_H___
#define __HITWORD_H___

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define InRange(x,a,b)    ((x)>=(a) && (x)<(b))
#define MAX(a,b) ((a)>(b)?(a):(b)) 

typedef int BOOL;
#define FALSE 0  
#define TRUE  1  

typedef enum { // user defined event 
	evtStartGame=1,			// renew a game 
	evtStopMusic,			// stop music
	evtGameFail				// the game is lost 
} UserEvent;

typedef struct
{
	int x;
	int y;
} Coord2D;

typedef struct { 

	SDL_Texture* mTexture;  
	int mWidth;
	int mHeight;
}LTexture;
  
typedef struct{
	LTexture gback;
	int scrollingOffset;
}ScolBack;	//Scrolling Backgrounds

typedef struct{
	LTexture gufo; 
	SDL_Rect rect;	//用于碰撞检测
}UFO;	

typedef struct{
	BOOL valid;
	int frame;
	SDL_Rect rect;
}Explosion;

typedef struct{

	SDL_Rect rect;
	int angle;
	int aim; 
	Coord2D vel; 
}Bomb;

typedef struct 
{
	char entry[20];			/* 条目, 单词最长20个字母; 当单词刚好20个字母时, 不用'\0'结束 */
	long lib_offset;		/* 单词解释的偏移量(该偏移量是指字典正文文件dict.lib中的fseek距离) */
	short int  xlat_len;	/* 单词解释占用的字节数 */
	short int  exam_len;	/* 单词例句占用的字节数 */
} IDX; 

typedef struct 
{ 
	char word[21];		//单词内容	
	SDL_Rect rect;		//用于碰撞检测  
} WORD;

typedef struct{
	int hp;			//实际生命值
	int screen_hp;	//屏幕显示值
	LTexture gblood;
	LTexture gbloodBg;
}Blood;

typedef struct
{   
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	SDL_Window *gWindow;
	SDL_Renderer *gRenderer; 
	TTF_Font *gFont;	
	Uint32 timeout;		//用于定时生成单词
	Blood blood;
	BOOL gameover;
	//音效
	Mix_Music *gMusic;
	Mix_Chunk *wavFire;
	Mix_Chunk *wavBomb;
	//纹理
	LTexture gbomb;
	LTexture gplanet; 
	LTexture gexplosion;
	LTexture gletters[52];	//26个字母，两种颜色，共52个不同纹理的字符
	LTexture gGameOver1;	//游戏失败标题
	LTexture gGameOver2;	//游戏失败提示

	ScolBack background;
	UFO ufo;
	Bomb bombs[10];			//游戏中最大允许有10个炮弹同时发出
	int screen_bombs;		//屏幕上现存炮弹数
	Explosion expls[10];	//爆炸序列帧	 
	int nExpSprite;			//爆炸序列总帧数

	WORD word[25];		// 显示在屏幕上的单词
	int screen_words;	// 屏幕上已显示的单词个数
	int hit,hitlen;		//当前拼写单词的数组下标，拼写正确的字母数 
	IDX *pdic;			// 字典中首个单词指针 
	int index[26][2];	// index[i][0]: ('a'+i)字母开头的首个单词在字典中的序号 index[i][1]: ('a'+i)字母开头的单词个数       
	    
} MineApp;

extern MineApp theApp;

//=========================以下在Main.c中实现========================// 
int InitApp();
void CleanApp();

//=========================以下在Util.c中实现========================//  
int explsWidth();
int explsHeight();
int planetWidth();
int planetHeight();
BOOL LoadMedia();
void MakeRect(SDL_Rect *rect,int x,int y,int w,int h); 
BOOL IsConflict(SDL_Rect rect1,SDL_Rect rect2);

//=========================以下在LTexture.c中实现====================//  
LTexture load_image(char* path); 
LTexture load_TTF_text( char *text,TTF_Font* gFont,SDL_Color color);
BOOL loadLetters();
void freeLTexture(LTexture *ltexture); 
void renderTexture(LTexture ltexture,int x,int y);

//=========================以下在Event.c中实现=======================// 
void RunApp();
void postUserEvent(int code);
void routeEvent(SDL_Event* event);
 
//=========================以下在Draw.c中实现========================// 
void DrawBackground();
void renderApp();
 
//=========================以下在UFO.c中实现=========================// 
int UFOPosX();
int UFOPosY(); 
int UFOWidth();
int UFOHeight();
int UFOHP();
int UFOScreenHP();
int HPWidth();
int HPHeight();
//=========================以下在Word.c中实现========================// 
int read_dic(IDX **ppdic); 
int wordX(int index);
int wordY(int index);
int wordWidth(int index);
int wordHeight(int index);  
void generate_one_word();
void delete_one_word(int wordIndex);  
void draw_one_word(int wordIndex);
void updateWords();
void clearWords();

//=========================以下在Bomb.c中实现========================// 
int bombY(int bombIndex);
int bombX(int bombIndex);
int bombW(int bombIndex);
int bombH(int bombIndex);
int bombAim(int bombIndex);
int bombAngle(int bombIndex);
void fire_a_Bomb(int wordIndex);
void fire_a_Explosion(int x,int y);
void updateBombs();
void clearBombs();

#endif