#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "util.h"

#ifdef _WIN
#pragma warning(disable:4996) 
#endif

char gMediaPath[256] = "../Media";
/*定义窗口位置(100,100),大小(640,480)*/
SDL_Rect  gMainWinRect = { 100, 100, 640, 480 };
/* 窗口指针变量 */
SDL_Window   * gMainWindow = NULL;
/* 图形绘制环境指针 */
SDL_Renderer * gMainRenderer = NULL;

int main(int argc, char *argv[]) 
{
	SDL_Color textcolor = { 255, 255, 0, 255 }; //黑色
	if( argc==2 ) strcpy(gMediaPath, argv[1]);
	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	// Create a window via SDL
	gMainWindow = SDL_CreateWindow("Draw Text using SDL_ttf", 
			gMainWinRect.x, gMainWinRect.y, gMainWinRect.w, gMainWinRect.h, 0);
	// Create rendering context
	gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED);
	// 初始化TTF子系统
	TTF_Init();
	// 先清除原来的所有内容
	SDL_SetRenderDrawColor(gMainRenderer, 128, 128, 128, 255); // 灰色
	SDL_RenderClear(gMainRenderer); //清屏
	drawstring("Hi, there!", gMainWinRect.w/8,20, 
		FullPath("/default/FreeSerif.ttf"), 64, textcolor);
	textcolor.r = 0; // 红色
	drawstring("Can you see me?", gMainWinRect.w/8,gMainWinRect.h-100, 
		FullPath("/default/FreeSerif.ttf"), 64, textcolor);
	// 将绘制内容呈现出来
	SDL_RenderPresent(gMainRenderer); 

	// Enter event processing loop 
	while ( 1 ) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break; // 终止应用程序
			}
		}
	}
	// Destroy and quit
	TTF_Quit(); // 退出TTF子系统
	SDL_DestroyRenderer(gMainRenderer);
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
	// Finished!
	return 0;
}
