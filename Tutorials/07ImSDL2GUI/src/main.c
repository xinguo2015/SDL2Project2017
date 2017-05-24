#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"

#include "mygui.h"


#if (defined _WIN32) || (defined _WIN64)

#pragma warning(disable:4996) 

#endif


#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

int            gGameover = 0;
char           gMediaPath[256] = "../Media";
SDL_Rect       gMainRect = { 100, 100, 960, 720 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
TTF_Font *     gMainFont = NULL;

char * MakePath(char path[], char dir[], char fname[])
{
	strcpy(path,dir);
	return strcat(path,fname);
}

static void cleanAll()
{
	if( gMainFont ) TTF_CloseFont(gMainFont), gMainFont = NULL;
	if( gMainRenderer ) SDL_DestroyRenderer(gMainRenderer), gMainRenderer = NULL;
	if( gMainWindow ) SDL_DestroyWindow(gMainWindow), gMainWindow = NULL;
}

int openFont(char fontFile[], int fontSize)
{
	TTF_Font * ft = TTF_OpenFont(fontFile, fontSize);//打开字体
	if( !ft ) return 0;
	if( gMainFont ) TTF_CloseFont(gMainFont);
	gMainFont = ft;
	return 1;
}

void initApp(int argc, char *argv[])
{
	char ttfFile[128];
	SDL_SetHint(SDL_HINT_IME_INTERNAL_EDITING, "1" );
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS );
	// 创建窗口
	if( ! (gMainWindow = SDL_CreateWindow(argv[0], gMainRect.x, gMainRect.y, gMainRect.w, gMainRect.h, 0) ) ) {
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
	// open a font file
	if( ! openFont(MakePath(ttfFile, gMediaPath,"/default/FreeSerif.ttf"), 28) ) {
		printf("Couldn't open font file: %s\n",ttfFile);
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

// GUI设计到的一些变量。它们不能是doUI的局部变量， 
//   否则每一次调用doUI处理的都是不同的变量。
// 这里我们将他们定义为全局变量。

char editstring[50] = "Here to set window title";
int  bgcolor        = 90|(255<<8)|(255<<16);
int  checkboxValue  = 1;
int  radiovalue     = 1;
int  liststart      = 0;
int  listvalue      = 1;
char*listitems[]    = { "1 Apple", 
						"2 Pear", 
						"3 Grape", 
						"4 Rice", 
						"5 Water", 
						"6 Salt", 
						"7 Drink", 
						"No MORE",
};


void doUI()
{
	imgui_prepare(); 
	{
		int x = 30, y = 50, w = 80, h = 48;
		int k, R, G, B;
		double slidervalue;
		char temp[64];

		// put a text label 
		textlabel(GenUIID(0), x, y, "Below are 10 buttons generated in a for loop"); y+= 50;
		// put an array of buttons
		for( k = 0; k<10; k++ ) // 10 buttons
			button(GenUIID(k), x+(w+10)*k, y, w, h, "array");
		// two more button
		y += 100;
		button(GenUIID(0), x,     y, w, h, "Click");  //put a button 
		button(GenUIID(0), x+100, y, w, h, "me");     //put a button 
		// another button
		if (button(GenUIID(0), x, y+100, w, h, "color" ))
			bgcolor = SDL_GetTicks() * 0xc3cac51a;
		// a quit button
		if (button(GenUIID(0), x+100, y+100, w, h, "quit")) { 
			// this button is clicked, quit the program
			SDL_Event ev;
			ev.type = SDL_QUIT; 
			// if call exit to quit, then no chance to clean app stuff
			// so, we push a quit event to the event queue
			SDL_PushEvent( &ev );
		}

		// slider bars
		x = 300; w = 22; h = 200;
		// a slider bar to tune the R channel of the background color
		slidervalue = bgcolor & 0xff; 
		if( slider(GenUIID(0), x, y, w, h, 0, 255, 1, & slidervalue)) 
			bgcolor = (bgcolor & 0xffff00) | (int)slidervalue;
		// a slider bar to tune the G channel of the background color
		slidervalue = (bgcolor >> 8) & 0xff;
		if( slider(GenUIID(0), x+50, y, w, h, 0, 255, 1, & slidervalue) )
			bgcolor = (bgcolor & 0xff00ff) | ((int)slidervalue) << 8;
		// a slider bar to tune the B channel of the background color
		slidervalue = (bgcolor >> 16) & 0xff;
		if (slider(GenUIID(0), x+100, y, w, h, 0, 255, 1, & slidervalue))
			bgcolor = (bgcolor & 0x00ffff) | ((int)(slidervalue) << 16);
		// show the value of the background color
		sprintf(temp, "(%3d,%3d,%3d)", bgcolor & 0xff, (bgcolor>>8) & 0xff, (bgcolor>>16) & 0xff);
		textlabel(GenUIID(0), x, y+h, temp);
		// a check box to toggle editing brightness
		checkbox( GenUIID(0), x+240, y+40, 30, 30, "Edit Brightness", & checkboxValue);
		if( checkboxValue )
		{
			// a slider bar to tune the brightness of the background color
			R = bgcolor & 0xff;  G = (bgcolor >> 8) & 0xff; B = (bgcolor >> 16) & 0xff;
			slidervalue = (R + G + B)/3;
			if (slider(GenUIID(0), x+150, y+h/2, h*2, w, 0, 255, 1, & slidervalue)) {
				int chg = (int)slidervalue - (R + G + B)/3;
				R = CLAMP(R+chg,0,255);
				G = CLAMP(G+chg,0,255);
				B = CLAMP(B+chg,0,255);
				bgcolor = R | (G<<8) | (B<<16);
			}
			sprintf(temp, "brightness = %d", (R+G+B)/3);
			textlabel(GenUIID(0), x+240, y+h/2+30, temp);
		}
		// a text input box
		x = 30; y += h+50 ;
		if( textbox(GenUIID(0), x, y, 500, 40, editstring, sizeof(editstring)-1) ) {
			// text is changed, you can do something here ...
			SDL_SetWindowTitle(gMainWindow,editstring);
		}
		// draw a box frame
		SDL_RenderDrawLine(gMainRenderer, x-5,   y+70,     x+250, y+70);
		SDL_RenderDrawLine(gMainRenderer, x-5,   y+70+130, x+250, y+70+130);
		SDL_RenderDrawLine(gMainRenderer, x-5,   y+70,     x-5,   y+70+130);
		SDL_RenderDrawLine(gMainRenderer, x+250, y+70,     x+250, y+70+130);
		// a group of radio buttons
		radio(GenUIID(0), x, y+ 80, 30, 30, "Good",      1, &radiovalue);
		radio(GenUIID(0), x, y+120, 30, 30, "Excellent", 2, &radiovalue);
		radio(GenUIID(0), x, y+160, 30, 30, "Great, wonderful",     3, &radiovalue);

		// a list box
		y -= 10;
		listbox(GenUIID(0), x+300, y+ 80, 200, 32*4, listitems, sizeof(listitems)/sizeof(listitems[0]), &liststart, &listvalue);
		sprintf(temp, "You selected: %s", listitems[listvalue]);
		textlabel(GenUIID(0), x+300, y+200, temp);
	}
	imgui_finish();
}

void display()
{
	SDL_SetRenderDrawColor(gMainRenderer, bgcolor&0xff, (bgcolor>>8)&0xff, (bgcolor>>16)&0xff, (bgcolor>>24)&0xff);
	SDL_RenderClear(gMainRenderer);
	// do UI part
	doUI();
	// below main rendering job
	//   current we have nothing to do
	// present the result
	SDL_RenderPresent(gMainRenderer);
}

void runMainLoop()
{
	imgui_init();
	imgui_renderer(gMainRenderer);
	imgui_font(gMainFont);

	SDL_StartTextInput();
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
			// UI event
			imgui_handle( &e );
		}
		// other tasks
		display();
		// 延时10ms，避免独霸CPU
		SDL_Delay(10); 
	}
	SDL_StopTextInput();
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
	runMainLoop();// Enter main loop 
	endApp();

	return 0;
}
