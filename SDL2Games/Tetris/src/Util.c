/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "Russia.h" 	 

/************************************************************************/
/*函数原型：LTexture load_image(char path[])							*/                                                                   
/*传入参数：图片资源在硬盘上的相对路径									*/                                                                   
/*返 回 值：LTexture													*/                                                                   
/*函数功能：加载在硬盘上的图片资源，生成纹理			 				*/                                                                   
/************************************************************************/
LTexture load_image(char path[]){

	LTexture ltexture;

	SDL_Surface* loadedSurface=IMG_Load(path);

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
/*函数原型：LTexture load_TTF_Text( char text[])	 					*/                                                                   
/*传入参数：待显示的字符数组											*/                                                                   
/*返 回 值：LTexture													*/                                                                   
/*函数功能：生成字符串的纹理						 					*/                                                                   
/************************************************************************/  
LTexture load_TTF_text( char *text){

	SDL_Surface* textSurface; 
	LTexture textTexture; 
	SDL_Color textColor = { 240, 240, 0, 255 };  
	textSurface = TTF_RenderText_Solid( theApp.gFont, text, textColor ); 
	if( textSurface != NULL )
	{ 
		textTexture.mTexture = SDL_CreateTextureFromSurface( theApp.gRenderer, textSurface );
		if( textTexture.mTexture == NULL ) 
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() ); 
		else{
			textTexture.mWidth=textSurface->w;
			textTexture.mHeight=textSurface->h;
		}
		SDL_FreeSurface( textSurface );
	}
	else 
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() ); 

	return textTexture;
} 
 
/************************************************************************/
/*函数原型：LTexture load_Number( int num)			 					*/                                                                   
/*传入参数：待显示的数字												*/                                                                   
/*返 回 值：LTexture													*/                                                                   
/*函数功能：生成数字的纹理							 					*/                                                                   
/************************************************************************/ 
LTexture load_Number( int num){
	 
	char numText[10]; 
	sprintf(numText,"%d",num);   
	return load_TTF_text(numText);
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
/*返 回 值：媒体资源是否全部加载成功   									*/                                                                   
/*函数功能：加载游戏中用到的媒体资源					 				*/                                                                   
/************************************************************************/
BOOL LoadMedia(){
	 
	//加载字体
	theApp.gFont = TTF_OpenFont( FullPath("/media/myFont.ttf"), 28 );
	if( theApp.gFont == NULL ) 
	{
		printf("Failed to load TTF");
		return FALSE;
	} 
	  
	//加载游戏背景纹理
	theApp.imgBackground=load_image(FullPath("/media/pics/background.jpg") );
	if (!theApp.imgBackground.mTexture)
	{
		printf("Failed to load imgBackground");
		return FALSE;
	}
	
	//加载失败纹理
	theApp.imgFail=load_image(FullPath("/media/pics/failbg.jpg") );
	if (!theApp.imgFail.mTexture)
	{
		printf("Failed to load failbg");
		return FALSE;
	}
	
	//加载方块纹理
	theApp.imgBox=load_image(FullPath("/media/pics/box.jpg") );
	if (!theApp.imgBox.mTexture)
	{
		printf("Failed to load imgBox");
		return FALSE;
	} 
	 
	//加载开始键纹理
	theApp.StartBtn.ltexture=load_image(FullPath("/media/pics/startPic.png") );
	if (!theApp.StartBtn.ltexture.mTexture)
	{
		printf("Failed to load StartBtn");
		return FALSE;
	} 
	 
	//加载暂停键纹理
	theApp.StopBtn.ltexture=load_image(FullPath("/media/pics/stopPic.png") );
	if (!theApp.StopBtn.ltexture.mTexture)
	{
		printf("Failed to load StopBtn");
		return FALSE;
	}  
	 
	//加载退出键纹理
	theApp.QuitBtn.ltexture=load_image(FullPath("/media/pics/quitPic.png") );
	if (!theApp.QuitBtn.ltexture.mTexture)
	{
		printf("Failed to load QuitBtn");
		return FALSE;
	}
	//加载图形落地时的音频
	theApp.audio_attach = Mix_LoadWAV( FullPath("/media/audio/attach.wav") );
	if( !theApp.audio_attach)
	{
		printf( "Failed to load attatch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
	//加载鼠标进入按钮时的音频
	theApp.audio_levelUp = Mix_LoadWAV( FullPath("/media/audio/levelUp.wav") );
	if( !theApp.audio_levelUp)
	{
		printf( "Failed to load levelUp sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
	//加载鼠标按下按钮时的音频
	theApp.audio_btnDown = Mix_LoadWAV( FullPath("/media/audio/btnDown.wav") );
	if( !theApp.audio_btnDown)
	{
		printf( "Failed to load btnDown sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}
	//加载得分时的音频
	theApp.audio_score = Mix_LoadWAV( FullPath("/media/audio/score.wav") );
	if( !theApp.audio_score)
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	} 
	//加载失败时的音频
	theApp.audio_fail = Mix_LoadWAV( FullPath("/media/audio/fail.wav") );
	if( !theApp.audio_fail)
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		return FALSE;
	}

	theApp.nStartBtnSprite=4;
	theApp.nStopBtnSprite=8;
	theApp.nQuitBtnSprite=3; 
	theApp.nBoxSprite=7;

	return TRUE;
}
 
/************************************************************************/
/*函数原型：int BoxWidth()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：方块宽度					   								*/                                                                   
/*函数功能：获取游戏中单个方块的宽度									*/                                                                   
/************************************************************************/
int BoxWidth(){ 

	return theApp.imgBox.mWidth;
}

/************************************************************************/
/*函数原型：int BoxHeight()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：方块高度					   								*/                                                                   
/*函数功能：获取游戏中单个方块的高度									*/                                                                   
/************************************************************************/
int BoxHeight(){ 

	return theApp.imgBox.mHeight/theApp.nBoxSprite;
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

/************************************************************************/
/*函数原型：BOOL IsConflict(int BlockIndex,int x,int y)					*/                                                                   
/*传入参数：形状的索引BlockIndex，横坐标x,纵坐标y						*/                                                                   
/*返 回 值：有冲突返回TRUE,无冲突返回FALSE								*/                                                                   
/*函数功能：判断形状是否能存在于坐标（x,y）处							*/                                                                   
/************************************************************************/ 
BOOL IsConflict(int BlockIndex,int x,int y){

	int i;   
	for (i=0;i<=7;i +=2)
	{
		//**1.超过左右边界则冲突**/
		if (arrayBLOCK[BlockIndex].arrXY[i]+x<1||arrayBLOCK[BlockIndex].arrXY[i]+x>BOARD_WIDTH)
			return TRUE;

		/**2.如果方块还没落入游戏面板，则不必检查**/
		if (arrayBLOCK[BlockIndex].arrXY[i+1]+y<1)
			continue;

		/**3.该方块当前位置已存在于游戏面板中，则返回冲突**/
		if (theApp.Gameboard[arrayBLOCK[BlockIndex].arrXY[i]+x][arrayBLOCK[BlockIndex].arrXY[i+1]+y]>BOX_BLANK)
			return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/*函数原型：void KillLine(int y)										*/                                                                   
/*传入参数：纵坐标y														*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：消去第y行													*/                                                                   
/************************************************************************/  
void KillLine(int y){

	int i,j;
	for (j=y;j>=1;j--) 
		for (i=1;i<=BOARD_WIDTH;i++) 
			theApp.Gameboard[i][j]=theApp.Gameboard[i][j-1];  
}

/************************************************************************/
/*函数原型：BOOL IsLineFull(int y)										*/                                                                   
/*传入参数：纵坐标y														*/                                                                   
/*返 回 值：填满返回TURE,否则返回FALSE									*/                                                                   
/*函数功能：判断第y行是否已被填满 										*/                                                                   
/************************************************************************/ 
BOOL IsLineFull(int y){

	int i;
	for(i=1;i<=BOARD_WIDTH;i++)
		if (theApp.Gameboard[i][y]==BOX_BLANK)
			return FALSE;
	return TRUE;	
}

/************************************************************************/
/*函数原型：int KillLines(int y)										*/                                                                   
/*传入参数：纵坐标y														*/                                                                   
/*返 回 值：消去的行数													*/                                                                   
/*函数功能：消去第y行以及第y行之上连续的被填满的行						*/                                                                   
/************************************************************************/  
int KillLines(int y){

	int i,LinesKilled=0;

	for(i=0;i<4;i++)
	{
		while (IsLineFull(y))
		{
			KillLine(y);
			LinesKilled++;
			i++; 
		}
		y--;
		if (y<1)
			break; 
	}
	return LinesKilled;
}
 
/************************************************************************/
/*函数原型：BOOL IsGameOver()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：游戏结束返回TRUE,否则返回FALSE								*/                                                                   
/*函数功能：判断游戏是否结束											*/                                                                   
/************************************************************************/  
BOOL IsGameOver(){
	int i;
	for (i=1;i<=BOARD_WIDTH;i++)
	{
		if(theApp.Gameboard[i][1])
			return TRUE;
	}
	return FALSE;
}			

/************************************************************************/
/*函数原型：int IsTimeUp(int timecount)									*/                                                                   
/*传入参数：时间(单位毫秒)												*/                                                                   
/*返 回 值：时长超指定值则返回TRUE.否则返回FALSE 						*/                                                                   
/*函数功能：判断开始计时后时间间隔是否超过指定值						*/                                                                   
/************************************************************************/ 
 BOOL IsTimeUp(int timecount){  

	 if ( (SDL_GetTicks()-theApp.time)>timecount)
	 {
		 theApp.time=SDL_GetTicks();
		 return TRUE;
	 }

	 return	FALSE; 
 }

 /************************************************************************/
 /*函数原型：BOOL HitRectTest(int x,int y,SDL_Rect rect)				 */                                                                   
 /*传入参数：X坐标，Y坐标，矩形区域										 */                                                                   
 /*返 回 值：坐标在矩形区域内返回TRUE，否则返回FALSE 					 */                                                                   
 /*函数功能：判断坐标是否在矩形区域内									 */                                                                   
 /************************************************************************/ 
 BOOL HitRectTest(int x,int y,SDL_Rect rect){

	 return (x>=rect.x && y>=rect.y && x<rect.x+rect.w && y<rect.y+rect.h);
 }

 /************************************************************************/
 /*函数原型：void* getFocus()											 */                                                                   
 /*传入参数：无															 */                                                                   
 /*返 回 值：指向当前活跃BUTTON的指针  									 */                                                                   
 /*函数功能：获取指向当前鼠标活动所处BUTTON的指针						 */                                                                   
 /************************************************************************/ 
 void* getFocus(){

	 return theApp.pFocus;
 }
  
 /************************************************************************/
 /*函数原型：void* setFocus												 */                                                                   
 /*传入参数：指向当前活跃BUTTON的指针 									 */                                                                   
 /*返 回 值：指向当前活跃BUTTON的指针  									 */                                                                   
 /*函数功能：设定pFocuse指针为pCtrl										 */                                                                   
 /************************************************************************/ 
 void* setFocus(void* pCtrl){

	 theApp.pFocus=pCtrl;

	 return pCtrl;
 }
