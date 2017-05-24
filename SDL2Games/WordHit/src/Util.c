/****************************************************************** 
File name: Util.c
Date: 2015/7/17
Description: 通用工具类函数 
*******************************************************************/
#include "hitword.h"

/************************************************************************/
/*函数原型：int planetWidth()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：行星纹理的宽度										   		*/                                                                   
/*函数功能：获取行星纹理的宽度											*/                                                                   
/************************************************************************/
int planetWidth(){

	return theApp.gplanet.mWidth;
}

/************************************************************************/
/*函数原型：int planetHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：行星纹理的高度										   		*/                                                                   
/*函数功能：获取行星纹理的高度											*/                                                                   
/************************************************************************/
int planetHeight(){

	return theApp.gplanet.mHeight;
}

/************************************************************************/
/*函数原型：int explsWidth()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：爆炸纹理的宽度										   		*/                                                                   
/*函数功能：获取爆炸纹理的宽度											*/                                                                   
/************************************************************************/
int explsWidth(){

	return theApp.gexplosion.mWidth/theApp.nExpSprite;
}

/************************************************************************/
/*函数原型：int explsHeight()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：爆炸纹理的高度										   		*/                                                                   
/*函数功能：获取爆炸纹理的高度											*/                                                                   
/************************************************************************/
int explsHeight(){

	return theApp.gexplosion.mHeight;
}

/************************************************************************/
/*函数原型：BOOL IsConflict(SDL_Rect rect1,SDL_Rect rect2)				*/                                                                   
/*传入参数：包围盒rect1,包围盒rect2										*/                                                                   
/*返 回 值：发生碰撞返回TRUE,否则返回FALSE						   		*/                                                                   
/*函数功能：AABB包围盒碰撞检测											*/                                                                   
/************************************************************************/
BOOL IsConflict(SDL_Rect rect1,SDL_Rect rect2){

	if (rect1.x+rect1.w<rect2.x)
		return FALSE;
	if (rect1.x>rect2.x+rect2.w)
		return FALSE;
	if (rect1.y+rect1.h<rect2.y)
		return FALSE;
	if (rect1.y>rect2.y+rect2.h)
		return FALSE;

	return TRUE;

}
 
/************************************************************************/
/*函数原型：void MakeRect(SDL_Rect *rect,int x,int y,int w,int h)		*/                                                                   
/*传入参数：待修改的SDL_Rect，横坐标x,纵坐标y，宽度w，高度h				*/                                                                   
/*返 回 值：无						   									*/                                                                   
/*函数功能：设定入参SDL_Rect的坐标与宽高								*/                                                                   
/************************************************************************/
void MakeRect(SDL_Rect *rect,int x,int y,int w,int h){

	rect->x=x;
	rect->y=y;
	rect->w=w;
	rect->h=h; 
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
	SDL_Color textColor={255,84,0,255};
	//加载字体 
	theApp.gFont = TTF_OpenFont( FullPath("/media/Arial.ttf"), 16 );
	if( theApp.gFont == NULL ) 
	{
		printf("Failed to load TTF.\n");
	} 
	//读取字典文件 
	if (!read_dic(&theApp.pdic))
	{
		printf("Failed to read_dic.\n");
		return FALSE;
	} 
	//加载游戏背景纹理
	theApp.background.gback=load_image(FullPath("/media/pics/background.jpg") );
	if (!theApp.background.gback.mTexture)
	{
		printf("Failed to load background.\n");
		return FALSE;
	} 
	//加载星球纹理
	theApp.gplanet=load_image(FullPath("/media/pics/plane.png") );
	if (!theApp.gplanet.mTexture)
	{
		printf("Failed to load plane.\n");
		return FALSE; 
	} 
	//加载UFO纹理 
	theApp.ufo.gufo=load_image(FullPath("/media/pics/ufo.png") );
	if (!theApp.ufo.gufo.mTexture)
	{
		printf("Failed to load ufo.\n");
		return FALSE; 
	} 
	MakeRect( &theApp.ufo.rect,theApp.SCREEN_WIDTH/2-UFOWidth()/2,theApp.SCREEN_HEIGHT-UFOHeight(),UFOWidth(),UFOHeight());
	
	//加载血条纹理 
	theApp.blood.gblood=load_image(FullPath("/media/pics/blood.png") );
	if (!theApp.blood.gblood.mTexture)
	{
		printf("Failed to load gblood.\n");
		return FALSE; 
	} 
	theApp.blood.gbloodBg=load_image(FullPath("/media/pics/bloodBg.png") );
	if (!theApp.blood.gbloodBg.mTexture)
	{
		printf("Failed to load gbloodBg.\n");
		return FALSE; 
	} 
	//加载炮弹纹理
	theApp.gbomb=load_image(FullPath("/media/pics/bomb.png") );
	if (!theApp.gbomb.mTexture)
	{
		printf("Failed to load bomb.\n");
		return FALSE;
	} 

	//加载爆炸纹理
	theApp.gexplosion=load_image(FullPath("/media/pics/boom.png") );
	if (!theApp.gexplosion.mTexture)
	{
		printf("Failed to load gexplosion.\n");
		return FALSE;
	} 
	theApp.nExpSprite=20; 

	//加载两种颜色26个字母
	if (!loadLetters())
	{
		printf("Failed to load letters.\n");
		return FALSE;
	}
	//加载游戏失败文字提醒
	theApp.gFont = TTF_OpenFont( FullPath("/media/Arial.ttf"), 24 ); 
	theApp.gGameOver2=load_TTF_text("Please enter SPACE to restart or ESC to quit.",theApp.gFont,textColor);
	theApp.gFont = TTF_OpenFont( FullPath("/media/Semitalic.ttf"), 56 ); 
	theApp.gGameOver1=load_TTF_text("GAME OVER",theApp.gFont,textColor); 
	if (theApp.gGameOver1.mTexture==NULL||theApp.gGameOver2.mTexture==NULL)
	{
		printf("Failed to load gameover.\n");
		return FALSE;
	}

	//Load music
	theApp.gMusic = Mix_LoadMUS( FullPath("/media/audio/bgmusic.mp3") );
	if( theApp.gMusic == NULL )
	{
		printf( "Failed to load bgmusic music! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}

	theApp.wavFire = Mix_LoadWAV( FullPath("/media/audio/fire.wav") );
	if( theApp.wavFire == NULL )
	{
		printf( "Failed to load fire sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}

	theApp.wavBomb = Mix_LoadWAV( FullPath("/media/audio/bomb.wav") );
	if( theApp.wavBomb == NULL )
	{
		printf( "Failed to load bomb sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}

	return TRUE; 
}
