#ifndef _MAZE_H_
#define _MAZE_H_ 
#include "constant.h"



int threadFunction( void* data );
//======Event.c
void postUserEvent(int code);
void RunApp();

//======Draw.c
void renderApp();

//=====Map.c 
int partW();
int partH();
void generateMap(int row,int col);
BOOL touchesWall( SDL_Rect box, SDL_Rect tiles[], int totle_tiles );
int verticalTest(int x,int y,int velx);
int horizontalTest(int x,int y,int vely);
void resetGameBoard(int row,int col);

//=====Texture.c
LTexture load_image(char* path);
LTexture load_TTF_text( char *text,TTF_Font* gFont);
LTexture load_Number( int num);
void freeLTexture(LTexture *ltexture);
void renderTexture(LTexture ltexture,int x,int y);
void renderPart(LTexture ltexture,int id,int nsprite,int x,int y);

//======Util.c
BOOL LoadMedia();
BOOL checkCollision( SDL_Rect a, SDL_Rect b );
void MakeRect(SDL_Rect *rect,int x,int y,int w,int h);
BOOL IsSucceed();
BOOL GameOver();
int btnW();
int btnH();
void * GetFocus();
void * SetFocus(void * pCtrl);
void* GetControlAt(int x,int y);

//======Pumpkin.c 
int pumpkinX();
int pumpkinY();
void setpumpkinVelX(int velx);
void setpumpkinVelY(int vely);
void updatePumpkin();
void moveUp(int destY);
void moveDown(int destY);
void moveLeft(int destX);
void moveRight(int destX);

//======Mouse.c
void onLButtonDown(void* pCtrl);
void onLButtonUp(void* pCtrl);
void onMouseMove(void * pCtrls);

#endif