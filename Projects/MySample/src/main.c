#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "utilities.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

char           gMediaPath[256] = "";
SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
SDL_Color      gBackgroundColor = { 0, 0, 0, 255 };

static void cleanAll()
{
	if( gMainRenderer ) 
		SDL_DestroyRenderer(gMainRenderer), gMainRenderer = NULL;
	if( gMainWindow ) 
		SDL_DestroyWindow(gMainWindow), gMainWindow = NULL;
}

void initApp(int argc, char *argv[])
{
	SDL_SetHint(SDL_HINT_IME_INTERNAL_EDITING, "1" );
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS );
	// 创建窗口
	if( ! (gMainWindow = SDL_CreateWindow(argv[0], gMainWinRect.x, gMainWinRect.y,
		gMainWinRect.w, gMainWinRect.h, 0) ) ) {
		printf("Error create SDL window\n");
		cleanAll();	exit(-1);
	}
	// 创建绘制环境
	if( ! (gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, 
		SDL_RENDERER_ACCELERATED)) ) {
		printf("Error create SDL Renderer\n");
		cleanAll();	exit(-1);
	}
	// 初始化 true type font使用
	if ( TTF_Init() < 0 ) {
		printf("Couldn't initialize TTF: %s\n",SDL_GetError());
		cleanAll();	exit(-1);
	}
}

void endApp()
{
	TTF_Quit();
	SDL_DestroyRenderer(gMainRenderer);
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
}

void drawKochCurve(SDL_Renderer *gMainRenderer, double x0, double y0, double x1, double y1, int k)
{
    /*                            */
    /*            /\4             */
	/*           /  \             */
	/*     ____ /____\_____       */
	/*     0    2      3    1     */
	/*____________________________*/
	double x2, y2, x3, y3, x4, y4;
	x2 = 2.0/3 * x0 + 1.0/3 * x1;  
	y2 = 2.0/3 * y0 + 1.0/3 * y1;
	x3 = 1.0/3 * x0 + 2.0/3 * x1;
	y3 = 1.0/3 * y0 + 2.0/3 * y1;
	x4 = 1.0/2 * (x0 + x1) - sqrt(3.0)/6 * (y1 - y0);
	y4 = 1.0/2 * (y0 + y1) + sqrt(3.0)/6 * (x1 - x0);

	if( k > 1)    /*如果迭代次数大于1，就继续迭代下去，即执行以下程序*/
	{   /*对线段(x0, y0)和(x2, y2) 进行迭代运算，其余类同*/
		drawKochCurve(gMainRenderer, x0, y0, x2, y2, k - 1);  
		drawKochCurve(gMainRenderer, x2, y2, x4, y4, k - 1);
		drawKochCurve(gMainRenderer, x4, y4, x3, y3, k - 1);
		drawKochCurve(gMainRenderer, x3, y3, x1, y1, k - 1);
	} else {/*迭代次数等于1，停止迭代，用直线联结两点 */
		SDL_RenderDrawLine(gMainRenderer, (int)x0, (int)y0, (int)x2, (int)y2); 
		SDL_RenderDrawLine(gMainRenderer, (int)x2, (int)y2, (int)x4, (int)y4); 
		SDL_RenderDrawLine(gMainRenderer, (int)x4, (int)y4, (int)x3, (int)y3);
		SDL_RenderDrawLine(gMainRenderer, (int)x3, (int)y3, (int)x1, (int)y1); 
	}
}

void drawKochSnow()
{
	double posx = 450,  posy = 400,  csize = 150; 
	double height = sqrt(3.0)/2 * csize;
	double x[3] = {posx, posx+csize, posx+csize/2};
	double y[3] = {posy, posy, posy - height };
	int k;
	SDL_SetRenderDrawColor(gMainRenderer, 0, 255, 255, 255);
	for( k = 0; k<3; k++ ) {
		drawKochCurve(gMainRenderer, 
			x[k],y[k],x[(k+1)%3], y[(k+1)%3], 6);
	}
}

void drawCircle(int cenx, int ceny, int radius)
{
	int x0 = cenx+radius, y0 = ceny, N=100, k;
	SDL_SetRenderDrawColor(gMainRenderer, 128, 128, 0, 255);
	for( k=1; k<=N; k++ )
	{
		double angle = 2*M_PI / N * k;
		int x = (int)(cenx + radius*cos(angle));
		int y = (int)(ceny + radius*sin(angle));
		SDL_RenderDrawLine(gMainRenderer, x0,y0,x,y);
		x0 = x;
		y0 = y;
	}
}

void drawSolidCircle(int cenx, int ceny, int radius)
{
	int k;
	SDL_SetRenderDrawColor(gMainRenderer, 128, 0, 128, 255);
	for( k=0; k<=radius; k++ )
	{
		int x = (int)sqrt((double)radius*radius-k*k);
		SDL_RenderDrawLine(gMainRenderer, cenx-x, ceny+k, cenx+x, ceny+k);
		SDL_RenderDrawLine(gMainRenderer, cenx-x, ceny-k, cenx+x, ceny-k);
	}
}

void drawShapes()
{
	SDL_Rect rect = {50,50,100,100};
	// draw rectangle
	SDL_SetRenderDrawColor(gMainRenderer, 0, 0, 255, 255);
	SDL_RenderDrawRect(gMainRenderer,&rect);
	// another filled rectrangle
	SDL_SetRenderDrawColor(gMainRenderer, 0, 255, 0, 255);
	rect.x += rect.w + 20;
	SDL_RenderFillRect(gMainRenderer,&rect);
	// draw a triangle
	SDL_SetRenderDrawColor(gMainRenderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(gMainRenderer, 400, 50, 300, 150);
	SDL_RenderDrawLine(gMainRenderer, 300, 150, 500, 150);
	SDL_RenderDrawLine(gMainRenderer, 500, 150, 400, 50);
	// draw a circle
	drawCircle(100,350,80);
	// draw a filled circle
	drawSolidCircle(300, 350, 80);
	// draw a snow shape 
	drawKochSnow();
}

void display()
{
	SDL_SetRenderDrawColor(gMainRenderer, gBackgroundColor.r, 
		gBackgroundColor.g, gBackgroundColor.b, gBackgroundColor.a);
	SDL_RenderClear(gMainRenderer);
	// draw some geometry shapes
	drawShapes();
	// present the result
	SDL_RenderPresent(gMainRenderer);
}

int main(int argc, char *argv[]) 
{
	if( argc>1 ) strcpy(gMediaPath, argv[1]);
	initApp(argc,argv);
	// Enter event processing loop 
	display();
	while ( 1 ) 
	{
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT ) {
				break; // 终止应用程序
			}
		}
		SDL_Delay(10); // don't take all the cpu time
	}
	endApp();

	return 0;
}

