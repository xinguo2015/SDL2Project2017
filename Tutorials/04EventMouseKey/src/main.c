#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "util.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
SDL_Color      gBackgroundColor = { 0, 0, 0, 255 };
char gMediaPath[256] = "../Media";

static void cleanAll()
{
	if( gMainRenderer ) SDL_DestroyRenderer(gMainRenderer), gMainRenderer = NULL;
	if( gMainWindow ) SDL_DestroyWindow(gMainWindow), gMainWindow = NULL;
}

void initApp(int argc, char *argv[])
{
	SDL_SetHint(SDL_HINT_IME_INTERNAL_EDITING, "1" );
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS );
	// 创建窗口
	if( ! (gMainWindow = SDL_CreateWindow(argv[0], gMainWinRect.x, gMainWinRect.y, gMainWinRect.w, gMainWinRect.h, 0) ) ) {
		printf("Error create SDL window\n");
		cleanAll();	exit(-1);
	}
	// 创建绘制环境
	if( ! (gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED)) ) {
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

typedef struct 
{
	int mousex;     // x - mouse position
	int mousey;     // y - mouse position
	int mousedown;  // 1 - yes, 0 - no

	int keypressed; // key that was pressed
	int keymod;     // key modifier flags (such as shift pressed)
	int keychar;    // char that is input
}
UIState;

UIState uistate;


char* getKeyModName(char kmname[], int keymod)
{
	int n;
	int key[] = { KMOD_LCTRL, KMOD_RCTRL, KMOD_LSHIFT, KMOD_RSHIFT, KMOD_ALT };
	char* name[] = { " Left Control ", " Right Control ", " Left Shift ", " Right Shift "," Alt " }, *p;

	for(  p = kmname, n = 0; n<5; n++ ) {
		if( keymod & key[n] ) {
			strcpy(p, name[n]);
			p += strlen(p);
		}
	}
	p[0] = 0;
	return kmname;
}

void display()
{
	SDL_SetRenderDrawColor(gMainRenderer, gBackgroundColor.r, gBackgroundColor.g,
		gBackgroundColor.b, gBackgroundColor.a);
	SDL_RenderClear(gMainRenderer);
	{	// 显示 uistate
		SDL_Color textcolor = { 255, 255, 255, 255 };
		char fontfile[128], infotext[256], modname[32];
		int fontsize = 30, x, y;
		// 绘制文字
		MakePath(fontfile, gMediaPath, "/default/FreeSerif.ttf");
		drawstring("Key press state:", x = 10, y=10, fontfile, fontsize, textcolor);
		textcolor.r = 0;
		sprintf(infotext, "keypressed = %c", uistate.keypressed);
		drawstring(infotext, x+=20, y+=fontsize, fontfile, fontsize, textcolor);
		sprintf(infotext, "key mod = %s", getKeyModName(modname,uistate.keymod));
		drawstring(infotext, x, y+=fontsize, fontfile, fontsize, textcolor);
		sprintf(infotext, "key char = %c", uistate.keychar);
		drawstring(infotext, x, y+=fontsize, fontfile, fontsize, textcolor);
		sprintf(infotext, "mouse x = %d, y = %d", uistate.mousex, uistate.mousey);
		drawstring(infotext, x, y+=fontsize*2, fontfile, fontsize, textcolor);
		sprintf(infotext, "button down = %s", uistate.mousedown ? "yes" : "no");
		drawstring(infotext, x, y+=fontsize, fontfile, fontsize, textcolor);
		// draw a rect around the mouse
		drawrect(uistate.mousex-20, uistate.mousey-20, 40,40, 
			uistate.mousedown ? 0xFF00FF : 0xFFFF);
	}
	// present the result
	SDL_RenderPresent(gMainRenderer);
}

int handleEvent(SDL_Event* e)
{
	switch (e->type) {
	case SDL_MOUSEMOTION:
		// update mouse position
		uistate.mousex = e->motion.x;
		uistate.mousey = e->motion.y; 
		return 1;
	case SDL_MOUSEBUTTONDOWN:
		// update button down state if left-clicking
		if (e->button.button == 1) {
			uistate.mousedown = 1;
			return 1;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		// update button down state if left-clicking
		if (e->button.button == 1) {
			uistate.mousedown = 0;
			return 1;
		}
		break;
	case SDL_KEYDOWN:
		// If a key is pressed, report it to the widgets
		uistate.keypressed = e->key.keysym.sym;
		uistate.keymod = e->key.keysym.mod;
		return 1;
	case SDL_TEXTINPUT:
		uistate.keychar = e->text.text[0];
		return 1;
	}
	// 没有处理事件
	return 0; 
}

int main(int argc, char *argv[]) 
{
	if( argc<2 ) {
		printf("Usage: <media path>\n\n");
		printf("press enter to quit\n");
		getchar();	return (-1);
	}
	strcpy(gMediaPath, argv[1]);
	initApp(argc,argv);
	// Enter event processing loop 
	SDL_StartTextInput();
	display();
	while ( 1 ) 
	{
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT ) {
				break; // 终止应用程序
			}
			if( handleEvent( &e ) )
				display();
		}
		SDL_Delay(5); // don't take all the cpu time
	}
	SDL_StopTextInput();
	endApp();

	return 0;
}

