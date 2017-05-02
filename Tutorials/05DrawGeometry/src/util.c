#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"

#ifdef _WIN
#pragma warning(disable:4996)
#endif

extern SDL_Rect       gMainWinRect;
extern SDL_Window *   gMainWindow;
extern SDL_Renderer * gMainRenderer;
extern SDL_Color      gBackgroundColor;

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

SDL_Color Int2Color(int color)
{
	SDL_Color cc;
	cc.r = color&0xFF;
	cc.g = (color>>8)&0xFF;
	cc.b = (color>>16)&0xFF;
	cc.a = (color>>24)&0xFF;
	return cc;
}

int Color2Int(SDL_Color cc)
{
	return (cc.r&0xFF) | ((cc.g&0xFF)<<8) 
		| ((cc.b&0xFF)<<16) | ((cc.a&0xFF)<<24);
}


SDL_Texture * loadTextureFromBMP(char fileName[])
{
	SDL_Surface *surface = SDL_LoadBMP(fileName);
	SDL_Texture *texture = NULL;
	if( !surface ) return NULL;
	texture = SDL_CreateTextureFromSurface(gMainRenderer, surface);
	SDL_FreeSurface(surface);  
	return texture;
}

void drawrect(int x, int y, int w, int h, int color)
{
	SDL_Rect r = {x,y,w,h};
	SDL_Color cc = Int2Color(color);
	SDL_SetRenderDrawColor(gMainRenderer, cc.r, cc.g, cc.b, cc.a);
	SDL_RenderFillRect(gMainRenderer, &r);
}

SDL_Rect drawstring(char s[], // 文本字符串
	int x, int y,             // 绘制的位置
	char fontFile[],          // 指定字体文件
	int fontSize,             // 字体大小
	SDL_Color color)          // 颜色
{
	SDL_Rect rect = { 0,0,0,0 };
	TTF_Font * textFont;
	SDL_Surface* surface;
	SDL_Texture* texture;

	textFont = TTF_OpenFont(fontFile, fontSize); //载入字体
	if( ! textFont ) return rect; // 失败
	// 将文字绘制到一个临时缓存区(SDL_Surface)
	surface = TTF_RenderText_Blended(textFont, s, color);
	TTF_CloseFont(textFont); // 关闭字体
	if( !surface ) return rect; // 失败
	// 将临时surface载入高速显存纹理中
	texture = SDL_CreateTextureFromSurface(gMainRenderer,surface);
	SDL_FreeSurface(surface); // 销毁surface
	if( !texture ) return rect;
	// 显示文字纹理
	rect.x = x; rect.y = y; // 位置
	SDL_QueryTexture(texture,NULL,NULL, &rect.w, &rect.h); //尺寸
	SDL_RenderCopy(gMainRenderer, texture, NULL, &rect); // 显示
	SDL_DestroyTexture(texture); // 销毁纹理
	return rect; // 返回文本所占据的区域
}

