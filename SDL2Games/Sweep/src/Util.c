/****************************************************************** 
Description：本文件包含工具类函数，函数注释中的雷块表示游戏区域的每个方格，雷区表示这些方格组成的区域
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/

#include "miner.h"

/************************************************************************/
/*函数原型：void CalcLayout()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：计算游戏窗口大小，计时器等部件的Rect			 			*/                                                                   
/************************************************************************/
void CalcLayout()
{
	int w = theApp.SCREEN_WIDTH;
	int h = theApp.SCREEN_HEIGHT;
	int w1 = DigitsWidth()*6 + FacesWidth();
	int w2 = FieldsWidth();
	int w3 = w2>w1 ? w2 : w1;
	int h1 = DigitsHeight() > FacesHeight() ? DigitsHeight() : FacesHeight();
	int h2 = FieldsHeight() + h1;
	int mgY = h>h2 ? (h-h2)/3 : 0;
	int mgX = w>w3 ? (w-w3)/2 : 0;

	MakeRect(& theApp.taskButton.rect, mgX, mgY, 
		DigitsWidth()*3, DigitsHeight());
	MakeRect(& theApp.timeButton.rect, w - mgX - DigitsWidth()*3, mgY, 
		DigitsWidth()*3, DigitsHeight());
	MakeRect(& theApp.faceButton.rect, w/2-FacesWidth()/2, mgY, 
		FacesWidth(), FacesHeight());
	MakeRect(& theApp.mineField.rect, mgX, mgY + h1 + mgY,
		FieldsWidth(), FieldsHeight());
	theApp.taskButton.rect.x += 3;
	theApp.timeButton.rect.x -= 3;
}

/************************************************************************/
/*函数原型：LTexture load_image(char path[])							*/                                                                   
/*传入参数：图片资源在硬盘上的相对路径									*/                                                                   
/*返 回 值：LTexture													*/                                                                   
/*函数功能：加载在硬盘上的图片资源，生成纹理			 				*/                                                                   
/************************************************************************/
LTexture load_image(char path[]){

	LTexture ltexture;

	SDL_Surface* loadedSurface=SDL_LoadBMP(path);

	if(loadedSurface==NULL)
	{
		printf("Unable to load image %s!SDL Error: %s\n",path,SDL_GetError());
	} 
	else
	{  
		ltexture.mTexture=SDL_CreateTextureFromSurface(theApp.gRenderer,loadedSurface);

		if (ltexture.mTexture==NULL)
		{ 
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		} 
		else{
			ltexture.mWidth=loadedSurface->w;
			ltexture.mHeight=loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	} 

	return ltexture;	
}

/************************************************************************/
/*函数原型：void freeLTexture(LTexture *ltexture)		 				*/
/*传入参数：待释放的LTexture											*/
/*返 回 值：无															*/
/*函数功能：释放LTexture资源						 					*/
/************************************************************************/
void freeLTexture(LTexture *ltexture){

	if (ltexture->mTexture != NULL){

		SDL_DestroyTexture(ltexture->mTexture);
		ltexture->mWidth = 0;
		ltexture->mHeight = 0;
		ltexture->mTexture = NULL;
	}
}


extern char gMediaPath[256];

char * MakePath(char path[], char dir[], char filename[])
{
	strcpy(path,dir);
	return strcat(path, filename);
}

char *FullPath(char filename[])
{
	static char path[256];
	return MakePath(path, gMediaPath, filename);
}

/************************************************************************/
/*函数原型：BOOL LoadMedia()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：媒体全部加载成功返回TRUE,否则返回FALSE   					*/                                                                   
/*函数功能：加载游戏中用到的媒体资源					 				*/                                                                   
/************************************************************************/
BOOL LoadMedia(){
  
	theApp.imgFaces=load_image(FullPath("/media/pics/430.bmp"));
	if (!theApp.imgFaces.mTexture)
	{
		printf("Failed to load imgFaces");
		return FALSE;
	}
	 
	theApp.imgDigits=load_image(FullPath("/media/pics/420.bmp"));
	if (!theApp.imgDigits.mTexture)
	{
		printf("Failed to load imgDigits");
		return FALSE;
	}
	 
	theApp.imgBombs=load_image(FullPath("/media/pics/410.bmp"));
	if (!theApp.imgBombs.mTexture)
	{
		printf("Failed to load imgBombs");
		return FALSE;
	} 

	//加载开始游戏音频
	theApp.audio_start = Mix_LoadWAV( FullPath("/media/audio/start.wav") );
	if( !theApp.audio_start)
	{
		printf( "Failed to load start sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}

	//加载自动翻面音频
	theApp.audio_reveal = Mix_LoadWAV( FullPath("/media/audio/reveal.wav") );
	if( !theApp.audio_reveal)
	{
		printf( "Failed to load reveal sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}

	//加载爆炸音频
	theApp.audio_bomb = Mix_LoadWAV( FullPath("/media/audio/bomb.wav") );
	if( !theApp.audio_bomb)
	{
		printf( "Failed to load bomb sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}

	//加载胜利音频
	theApp.audio_win = Mix_LoadWAV( FullPath("/media/audio/win.wav") );
	if( !theApp.audio_win)
	{
		printf( "Failed to load win sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
 
	theApp.nFaceSprite = 5;
	theApp.nDigitSprite = 12;
	theApp.nBombSprite = 16;

	return TRUE;
}

/************************************************************************/
/*函数原型：int DigitsWidth()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：数字纹理宽度				   								*/                                                                   
/*函数功能：获取0-9数字纹理的宽度										*/                                                                   
/************************************************************************/
int DigitsWidth()
{
	return theApp.imgDigits.mWidth;
}

/************************************************************************/
/*函数原型：int DigitsHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：数字纹理高度				   								*/                                                                   
/*函数功能：获取0-9数字纹理的高度										*/                                                                   
/************************************************************************/
int DigitsHeight()
{
	return theApp.imgDigits.mHeight/theApp.nDigitSprite;
}

/************************************************************************/
/*函数原型：int CellsWidth()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：单个雷块的宽度				   								*/                                                                   
/*函数功能：获取单个雷块的宽度											*/                                                                   
/************************************************************************/
int CellsWidth()
{
	return theApp.imgBombs.mWidth;
}

/************************************************************************/
/*函数原型：int CellsHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：单个雷块的高度				   								*/                                                                   
/*函数功能：获取单个雷块的高度											*/                                                                   
/************************************************************************/
int CellsHeight()
{
	return theApp.imgBombs.mHeight/theApp.nBombSprite;
}

/************************************************************************/
/*函数原型：int FieldsWidth()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：雷区的宽度				   								    */                                                                   
/*函数功能：获取整个雷区的宽度											*/                                                                   
/************************************************************************/
int FieldsWidth()
{
	return CellsWidth() * theApp.mineField.fldSize[0];
}

/************************************************************************/
/*函数原型：int FieldsHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：雷区的高度				   								    */                                                                   
/*函数功能：获取整个雷区的高度											*/                                                                   
/************************************************************************/
int FieldsHeight()
{
	return CellsHeight() * theApp.mineField.fldSize[1];
}
 
/************************************************************************/
/*函数原型：int FacesWidth()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：卡通脸按钮的宽度				   							*/                                                                   
/*函数功能：获取卡通脸按钮的宽度										*/                                                                   
/************************************************************************/
int FacesWidth()
{
	return theApp.imgFaces.mWidth;
}

/************************************************************************/
/*函数原型：int FacesHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：卡通脸按钮的高度				   							*/                                                                   
/*函数功能：获取卡通脸按钮的高度										*/                                                                   
/************************************************************************/
int FacesHeight()
{
	return theApp.imgFaces.mHeight/theApp.nFaceSprite;
}

/************************************************************************/
/*函数原型：void MakeRect(SDL_Rect *rect,int x,int y,int w,int h)		*/                                                                   
/*传入参数：待修改的SDL_Rect，横坐标x,纵坐标y，宽度w，高度h				*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：设定入参SDL_Rect的坐标与宽高								*/                                                                   
/************************************************************************/
void MakeRect(SDL_Rect* rect, int x, int y, int w, int h)
{
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}

/************************************************************************/
/*函数原型：BOOL HitRectTest(int x, int y, SDL_Rect rect)				*/                                                                   
/*传入参数：待修改的SDL_Rect，横坐标x,纵坐标y，宽度w，高度h				*/                                                                   
/*返 回 值：坐标位于矩形区域内返回TRUE，否则返回FALSE					*/                                                                   
/*函数功能：判断坐标（x，y）是否位于矩形区域内							*/                                                                   
/************************************************************************/
BOOL HitRectTest(int x, int y, SDL_Rect rect)
{
	return (x>=rect.x && y>=rect.y && x<rect.x+rect.w && y<rect.y+rect.h );
}

/************************************************************************/
/*函数原型：void * GetFocus()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：获取焦点部件的指针（例如笑脸按钮）							*/                                                                   
/************************************************************************/
void * GetFocus()
{
	return theApp.pFocus;
}

/************************************************************************/
/*函数原型：void * SetFocus(void * pCtrl)								*/                                                                   
/*传入参数：空类型的指针												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定焦点为pCtrl 											*/                                                                   
/************************************************************************/
void * SetFocus(void * pCtrl)
{
	theApp.pFocus = pCtrl;
	return pCtrl;
}

/************************************************************************/
/*函数原型：void* GetControlAt(int x, int y)							*/                                                                   
/*传入参数：X坐标，Y坐标												*/                                                                   
/*返 回 值：坐标位于某个部件内返回其指针，否则返回空					*/                                                                   
/*函数功能：获取坐标（x，y）所在的部件 									*/                                                                   
/************************************************************************/
void* GetControlAt(int x, int y)
{
	if( HitRectTest(x,y,theApp.faceButton.rect) )
		return & theApp.faceButton;
	if( HitRectTest(x,y,theApp.mineField.rect) )
		return & theApp.mineField;
	return NULL;
}

/************************************************************************/
/*函数原型：BOOL IsCell(void *p)										*/                                                                   
/*传入参数：空类型的指针												*/                                                                   
/*返 回 值：指针所指部件是一个雷块返回TRUE，否则返回FALSE				*/                                                                   
/*函数功能：判断指针所指部件是否为雷块									*/                                                                   
/************************************************************************/
BOOL IsCell(void *p)
{
	MineField *fld = & theApp.mineField;
	void * plast = fld->pCells + fld->fldSize[2];
	return InRange(p, (void*)fld->pCells, plast);
}

/************************************************************************/
/*函数原型：Coord2D CellCoord(MineCell *p)								*/                                                                   
/*传入参数：雷块指针													*/                                                                   
/*返 回 值：指针所指雷块的坐标（以雷块宽度为单位）						*/                                                                   
/*函数功能：计算指定雷块的坐标											*/                                                                   
/************************************************************************/
Coord2D CellCoord(MineCell *p)
{
	MineField *fld = & theApp.mineField;
	Coord2D xy;
	int n = p - fld->pCells;
	xy.x = n % fld->fldSize[0];
	xy.y = n / fld->fldSize[0];
	return xy;
}
