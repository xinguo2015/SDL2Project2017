#include <string.h>
#include "SDL.h"
#include "utilities.h"

#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

int  gGameover = 0;
char gMediaPath[256] = "../Media";
/*定义窗口位置(100,100),大小(640,480)*/
SDL_Rect  gMainWinRect = { 100, 100, 640, 480 };
/* 窗口指针变量 */
SDL_Window   * gMainWindow = NULL;
/* 图形绘制环境指针 */
SDL_Renderer * gMainRenderer = NULL;

int main(int argc, char *argv[]) 
{
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;
	// store media path
	if( argc>1 ) 
		strcpy(gMediaPath, argv[1]);
	else {
		strcpy(gMediaPath, SDL_GetBasePath());
		strcat(gMediaPath, "../../../Media");
	}
	printf("base path = %s\n", SDL_GetBasePath());
	printf("media path = %s\n", gMediaPath);
	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	// Create a window via SDL
	gMainWindow = SDL_CreateWindow("Hello World", gMainWinRect.x, gMainWinRect.y, gMainWinRect.w, gMainWinRect.h, 0);
	// Load an image and set as the gMainWindow icon
	surface = SDL_LoadBMP(FullPath("/default/SDLLogo.bmp"));
	SDL_SetWindowIcon(gMainWindow, surface);
	SDL_FreeSurface(surface);
	// Create rendering context
	gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED);
	//读入一幅BMP图像
	surface = SDL_LoadBMP(FullPath("/default/helloSDL.bmp"));
	//将图像转换为高效率的纹理
	texture = SDL_CreateTextureFromSurface(gMainRenderer, surface);
	// 图像不再有用，释放掉
	SDL_FreeSurface(surface);  
	// Display the image
	// 先清除原来的所有内容
	SDL_RenderClear(gMainRenderer); //清屏
	// 显示图像
	SDL_RenderCopy(gMainRenderer, texture, NULL, NULL);
	// 将绘制内容呈现出来
	SDL_RenderPresent(gMainRenderer); 
	// Enter main loop 
	while ( !gGameover ) 
	{
		SDL_Event e; // 处理事件
		while ( !gGameover && SDL_PollEvent(&e)) 
		{
			if((e.type == SDL_KEYUP && e.key.keysym.sym==SDLK_ESCAPE) ||
				e.type == SDL_QUIT) //user close window or press ESC key
			{
				gGameover = 1; // 终止应用程序
			}
			// other events
		}
		// 做一些其他的事情。。。。。。。。
		SDL_Delay(10); // 延时10ms，避免独霸CPU
	}
	// Destroy and quit
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(gMainRenderer);
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
	// Finished!
	return 0;
}
