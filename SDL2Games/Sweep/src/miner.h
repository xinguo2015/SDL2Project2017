/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#ifndef __MINER_H___123456509_ertgddf_
#define __MINER_H___123456509_ertgddf_


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifdef _DEBUG // for debug
#define MyDebug(p) p   
#else // no debug
#define MyDebug(p)     
#endif

typedef int BOOL;
#define FALSE 0  
#define TRUE  1  

#define MAX_FIELD 100
#define InRange(x,a,b)    ((x)>=(a) && (x)<(b)) // is x in [a, b) interval ?
#define MIN(a,b)          ((a)<(b)?(a):(b))     // the minimal of a and b
#define MAX(a,b)          ((a)>(b)?(a):(b))     // the minimal of a and b
#define ABS(a)            ((a)<0?(a):-(a))      // absolute value
#define FlatIndex(x,y,NX) ((x)+(y)*(NX))        // convert (x,y) coord to a single index in the array, NX is the row size
#define BOMB              ((char)9)             // the value standing for the bomb
#define IsBomb(d)         (d==BOMB)             // check if a value is a bomb

typedef struct {

	SDL_Texture* mTexture; 
	int mWidth;
	int mHeight;
}LTexture;

typedef enum { // states for the mine field of the game
	sNewGame = 0, // a new game just generated
	sWait,        // waiting for you
	sLeftDown,    // received a left button down
	sRightDown,   // received a right button down
	sLRDown,      // received two button down (left and right)
	sGameOver,    // game is over (you win or lose)
} GameState;

typedef enum { // user defined event
	evtStartTimer = 1, // start timing
	evtStopTimer,      // stop timing
	evtNewGame,        // renew a game
	evtMarkMore,       // find one more bomb
	evtMarkLess,       // decrease one bomb
	evtFaceOp,         // show whoops face (o)
	evtFaceSmile,      // show smile face
	evtFaceSink,       // show sinked smile face
	evtGameFail,       // the game is lost
	evtGameWin         // win the game
} UserEvent;

typedef enum {      //the state of the cells
	csCovered = 0,  // covered
	csRevealed,     // revealed
	csAutomated,    // automatcially revealed the neighbor
	csBombFlag,     // flaged as bomb
	csQuestioned,   // question mark
	csPressed,      // pressed by mouse
	csPressedQ,     // questioned and pressed by mouse
} CellState;

typedef struct
{
	short x;
	short y;
} Coord2D;

typedef enum { // visual image index for the cells
	cvCovered = 0, // covered (not opened)
	cvFlag,        // a flag representing the bomb
	cvQsMark,      // a question mark on the cell
	cvRedBomb,     // a bomb blustering you
	cvWrongBomb,   // not a bomb, but you think it is a bomb
	cvBomb,        // a real bomb
	cvQsMarkSink,  // a question mark sinked
	cvNum8,        // the image for number 8
	cvTotal = 16   // the number of all images
} CellVisualID;

typedef enum {
	FaceSmileSink = 0, //
	FaceGlass ,        // face with glass for win the game
	FaceFail ,         // cry face for lose the game
	FaceOp ,           // whoops face for open a cell
	FaceSmile,         // smiling for waiting the user
	FaceTotal          // the number of the face expressions ( = 5 )
} FaceState;

typedef struct 
{
	char data;  // 9: a bomb; 0-8: the surrounding bomb number 
	char state; // one of the bst???? marcos
} MineCell;

typedef struct 
{   // the field of mines/bombs
	SDL_Rect   rect;        // the rectangle
	MineCell*  pCells;
	int        fldSize[3];
	int        cellSize[2];
	int        bombCount;
	GameState  state;
	MineCell*  pFocus;
} MineField;

typedef struct
{
	SDL_Rect rect;
	int data; // data on the button
} ImageButton;
 
typedef struct
{   
	SDL_Window *gWindow;
	SDL_Renderer *gRenderer;  
	LTexture imgFaces; // facial expressions
	LTexture imgDigits;// for shown digital numbers
	LTexture imgBombs; // states for each element in the mine field

	int nFaceSprite; //5
	int nDigitSprite;//12
	int nBombSprite; //16

	MineField    mineField;
	ImageButton  faceButton;
	ImageButton  taskButton;
	ImageButton  timeButton;
	SDL_TimerID  timerID;

	void * pFocus;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT; 
	int mouseXY[2];
	
	Mix_Chunk *audio_start;
	Mix_Chunk *audio_reveal;
	Mix_Chunk *audio_bomb;
	Mix_Chunk *audio_win;
	BOOL winNotPlay;
} MineApp;

extern MineApp theApp;
 
//=========================以下在Main.c中实现=======================// 
int InitApp();
void InitMineField();
void StartGame();
void CleanApp();

//=========================以下在Bomb.c中实现=======================// 
void GenerateRandomBombs(MineField *fld);
void CalcNeighbors(MineField *fld);
BOOL ConstructField(MineField * fld, int fldSizeX, int fldSizeY, int bombCount);
void DestroyField(MineField *fld);
int CellFinalVisualIndex(MineCell p);
int CellVisualIndex(MineCell p);
BOOL GameOver();
MineCell* GetCellAt(int x, int y);
void OpenCell(MineCell *pCell);
void LeftPressField(int x, int y, unsigned char flags);
void LeftReleaseField(int x, int y, unsigned char flags);
void RightPressField(int x, int y, unsigned char flags);
void RightReleaseField(int x, int y, unsigned char flags);
void MoveOnField(int x, int y, unsigned char flags);	
int MissionCompleted();

//=========================以下在Draw.c中实现=======================// 
void DrawFaceButton(ImageButton *b);
void DrawDigitButton(ImageButton *b);
void DrawBumpRect(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 thick, Uint8 dark, Uint8 bright);
void DrawFrames();
void DrawObject(void *p);
void renderApp();
  
//=========================以下在Event.c中实现=======================// 
void postUserEvent(int code);
void routeEvent(SDL_Event *event);
void RunApp();

//=========================以下在Util.c中实现=======================// 
void CalcLayout();
BOOL LoadMedia();
int DigitsWidth();
int DigitsHeight();
int FacesWidth();
int FacesHeight();
int CellsWidth();
int CellsHeight();
int FieldsWidth();
int FieldsHeight();
void MakeRect(SDL_Rect* rect, int x, int y, int w, int h);
int HitRectTest(int x, int y, SDL_Rect rect);
void * GetFocus();
void * SetFocus(void * pCtrl);
void * GetControlAt(int x, int y);
int IsCell(void *p);
Coord2D CellCoord(MineCell *p);
void freeLTexture(LTexture *ltexture);

//=========================以下在Mouse.c中实现=======================// 
void onLButtonDown(void *pCtrl, int x, int y, unsigned char flags);
void onLButtonUp(void *pCtrl, int x, int y, unsigned char flags);
void onRButtonDown(void *pCtrl, int x, int y, unsigned char flags);
void onRButtonUp(void *pCtrl, int x, int y, unsigned char flags);
void onMouseMove(void * pCtrl, int x, int y, unsigned char flags);

#endif //__MINER_H___123456509_ertgddf_
