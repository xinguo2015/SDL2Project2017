/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#ifndef _HANIO_H_
#define _HANIO_H_ 
#include "constant.h"


int threadFunction( void* data );
//==========================以下在Util.c中实现========================//
BOOL LoadMedia();
void * GetFocus();
void * SetFocus(void * pCtrl);
void* GetControlAt(int x,int y);
BOOL HitRectTest(int x,int y,SDL_Rect rect);
void MakeRect(SDL_Rect *rect,int x,int y,int w,int h);
BOOL IsBeginning();
BOOL IsWinning();
void moves(int t1,int t2);

//==========================以下在LTexture.c中实现========================//
LTexture load_image(char* path);  
LTexture load_TTF_text( char* text,TTF_Font* gFont,SDL_Color color);
LTexture load_Number( int num);
void freeLTexture(LTexture *ltexture) ;
void renderTexture(LTexture ltexture,int x,int y);

//==========================以下在Draw.c中实现========================//
void renderApp();

//==========================以下在Event.c中实现========================//
void postUserEvent(int code);
void RunApp();

//==========================以下在LNode.c中实现========================//
LNode* MakeNode(int index);
LNode* deleteNode(LNode* header);
void AddNode(LNode* header,LNode *node);
void RejoinNode(int x,int y);
void JoinNode(int t);
void detachNode(LNode* header,int x,int y);
void freeList(LNode* header);
void clearLists();
void initList();
void clearNodes();

//==========================以下在Mouse.c中实现========================//
void onLButtonDown(void* pCtrl,int x, int y, unsigned char flags);
void onLButtonUp(int x, int y, unsigned char flags);
void onMouseMove(void * pCtrl,int x, int y, unsigned char flags);
void LoseMouse(Button * pCtrl);

//==========================以下在Disc.c中实现========================//
int discW();
int discWidth();
int discHeight();
int towerWidth();
int towerHeight(); 
void setDiscs(int discs);
void incDiscs();
void decDiscs();
void setMove(int move);
void incMove();
BOOL IsDraging();
void* HitDiscTest(int x,int y);
BOOL IsDiscConflict(int x,int y,int index,SDL_Rect rect);
LNode* FindTouchedTower(int x,int y,int index);

//==========================以下在Button.c中实现========================//
int btnWidth();
int btnHeight();
void pressBtn(Button *pCtrl);
void releaseBtn(Button* pCtrl);

#endif