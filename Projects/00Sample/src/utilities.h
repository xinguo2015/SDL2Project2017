#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"

char * MakePath(char path[], char dir[], char filename[]);
char * FullPath(char filename[]);
void  drawrect(int x, int y, int w, int h, int color);
int   Color2Int(SDL_Color cc);
SDL_Color Int2Color(int color);
SDL_Rect drawstring(char s[], // 文本字符串
	int x, int y,             // 绘制的位置
	char fontFile[],          // 指定字体文件
	int fontSize,             // 字体大小
	SDL_Color color);         // 颜色
SDL_Texture* loadTextureFromBMP(char fileName[]);
