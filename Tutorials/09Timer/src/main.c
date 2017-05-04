#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "utilities.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

int            gGameover = 0;
char           gMediaPath[256] = "";
SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
SDL_Color      gBackgroundColor = { 0, 0, 0, 255 };
Uint32         gMyTimerEvent = (Uint32)-1;

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
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER );
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

typedef struct {
	float x,y;
} Vec2f;

typedef struct {
	Vec2f p[4];
} ClockPointer;

void drawline(Vec2f a, Vec2f b)
{
	SDL_RenderDrawLine(gMainRenderer, 
		(int)a.x, (int)a.y, (int)b.x, (int)b.y);
}

void drawPointer(Vec2f p[])
{
	int N = 30, k;
	for( k = 0; k<=N; k++ )
	{
		float t = k/(float)N;
		Vec2f v = {
			t * p[1].x + (1-t) * p[3].x,
			t * p[1].y + (1-t) * p[3].y};
		drawline(p[0], v   );
		drawline(v   , p[2]);
	}
}
Vec2f rotate00(float angle, Vec2f p)
{
	float cosa = (float)cos(angle);
	float sina = (float)sin(angle);
	Vec2f r = {
		cosa * p.x - sina * p.y,
		sina * p.x + cosa * p.y};
	return r;
}

Vec2f rotate(Vec2f center, float angle, Vec2f p)
{
	p.x -= center.x;
	p.y -= center.y;
	p = rotate00(angle,p);
	p.x += center.x;
	p.y += center.y;
	return p;
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
	for( k=0; k<=radius; k++ )
	{
		int x = (int)sqrt((double)radius*radius-k*k);
		SDL_RenderDrawLine(gMainRenderer, cenx-x, ceny+k, cenx+x, ceny+k);
		SDL_RenderDrawLine(gMainRenderer, cenx-x, ceny-k, cenx+x, ceny-k);
	}
}

void drawClock()
{
	time_t rawtime = time(NULL);
	struct tm tinfo = *localtime(&rawtime);
	char tstr[64];
	int clocksize = 160;
	float cx = gMainWinRect.w/2.f;
	float cy = gMainWinRect.h/2.f;
	float y1 = 18, dx = 12, y2 = -130;
	Vec2f p[4], pointer[3][4] = {
		{{0, y1},{dx-6, 0},{0, y2   },{-dx+6, 0}},
		{{0, y1},{dx-3, 0},{0, y2+15},{-dx+3, 0}},
		{{0, y1},{dx,   0},{0, y2+30},{-dx,   0}}};
	// draw clock
	SDL_SetRenderDrawColor(gMainRenderer, 255, 255, 255, 255);
	drawSolidCircle((int)cx, (int)cy, clocksize);
	SDL_SetRenderDrawColor(gMainRenderer, 128, 128, 128, 255);
	drawSolidCircle((int)cx, (int)cy, clocksize-5);
	SDL_SetRenderDrawColor(gMainRenderer, 255, 255, 255, 255);
	drawSolidCircle((int)cx, (int)cy, 8);
	// rotate the pointer of second
	// tinfo.tm_sec;
	{
		int k, j; double rotAngle[3] = {
			tinfo.tm_sec/60.0*M_PI*2,
			tinfo.tm_min/60.0*M_PI*2,
			tinfo.tm_hour%12/12.f*M_PI*2};
		for( j = 2; j>=0; j-- ) 
		{	
		    SDL_SetRenderDrawColor(gMainRenderer, 255*(j==0), 255*(j==1), 255*(j==2), 255);
			for( k = 0; k<4; k++ ) {
				p[k] = rotate00((float)rotAngle[j], pointer[j][k]);
				p[k].x += cx;
				p[k].y += cy;
			}
			drawPointer(p);
		}
	};
	strftime(tstr, sizeof(tstr), "%c",&tinfo);
	drawstring(tstr, gMainWinRect.w/5, gMainWinRect.h-50,
		FullPath("/default/FreeSerif.ttf"), 32, Int2Color(0xFFFFFFFF) );
}

void display()
{
	SDL_SetRenderDrawColor(gMainRenderer, gBackgroundColor.r, 
		gBackgroundColor.g, gBackgroundColor.b, gBackgroundColor.a);
	SDL_RenderClear(gMainRenderer);
	// draw some geometry shapes
	drawClock();
	// present the result
	SDL_RenderPresent(gMainRenderer);
}

Uint32 timerCallback(Uint32 interval, void* param)
{
	//display();
	//printf("timer\n");
	SDL_Event e;
	e.type = gMyTimerEvent;
	SDL_PushEvent(&e);
	return interval;
}
void runMainLoop()
{
	SDL_Event e; // 处理事件
	SDL_TimerID timerID;	
	// 注册一个时间类型, 增加一个定时器
	gMyTimerEvent = SDL_RegisterEvents(1); 
	timerID = SDL_AddTimer(1000, timerCallback, "this is my timer");
	display();
	while ( !gGameover ) 
	{
		while ( !gGameover && SDL_PollEvent(&e)) 
		{
			if((e.type == SDL_KEYUP && e.key.keysym.sym==SDLK_ESCAPE) ||
				e.type == SDL_QUIT) //user close window or press ESC key
			{
				gGameover = 1; // 终止应用程序
			}
			// other events ...
			if ( e.type == gMyTimerEvent )
				display();
		}
		// 做一些其他的事情。。。。。。。。
		SDL_Delay(10); // 延时10ms，避免独霸CPU
	}
	SDL_RemoveTimer( timerID );	
}

int main(int argc, char *argv[]) 
{
	if( argc>1 ) 
		strcpy(gMediaPath, argv[1]);
	else {
		strcpy(gMediaPath, SDL_GetBasePath());
		strcat(gMediaPath, "../../../Media");
	}
	printf("base path = %s\n", SDL_GetBasePath());
	printf("media path = %s\n", gMediaPath);

	initApp(argc,argv);
	runMainLoop(); // Enter main loop 
	endApp();

	return 0;
}

