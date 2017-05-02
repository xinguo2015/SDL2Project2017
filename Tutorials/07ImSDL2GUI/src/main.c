#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"

#include "mygui.h"

#pragma warning(disable:4996)

#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

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

int  updateUIState(SDL_Event* e);
void handleEvent(SDL_Event * e);
void display();

int main(int argc, char *argv[]) 
{
	if( argc>1 ) strcpy(gMediaPath, argv[1]);
	initApp(argc,argv);
	imgui_init();
	// Enter event processing loop 
	SDL_StartTextInput();
	display();
	while ( 1 ) 
	{
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break; // 终止应用程序
			}
			if( updateUIState( &e ) )
				display();
			else
				handleEvent( &e );
		}
		// don't take all the cpu time
		SDL_Delay(10); 
	}
	SDL_StopTextInput();
	endApp();

	return 0;
}

int updateUIState(SDL_Event* e)
{
	switch (e->type) {
	case SDL_MOUSEMOTION:
		// update mouse position
		gUIState.mousex = e->motion.x;
		gUIState.mousey = e->motion.y;
		return 1;
	case SDL_MOUSEBUTTONDOWN:
		// update button down state if left-clicking
		if (e->button.button == 1) {
			gUIState.mousedown = 1;
			return 1;
		}
		return 0;
	case SDL_MOUSEBUTTONUP:
		// update button down state if left-clicking
		if (e->button.button == 1) {
			gUIState.mousedown = 0;
			return 1;
		}
		return 0;
	case SDL_KEYDOWN:
		// If a key is pressed, report it to the widgets
		gUIState.keypressed = e->key.keysym.sym;
		gUIState.keymod = e->key.keysym.mod;
		return 1;
	case SDL_TEXTINPUT:
		gUIState.keychar = e->text.text[0];
		return 1;
	}
	return 0;
}

void handleEvent(SDL_Event *e)
{
	switch (e->type) {
	case SDL_KEYUP:
		switch (e->key.keysym.sym) {
		case SDLK_ESCAPE: 
			{
				SDL_Event ev;
				ev.type = SDL_QUIT; 
				SDL_PushEvent( &ev );
			}
		}
		break;
	default:
		break;
	}
}

void display()
{
	static char editstring[50] = "some text";
	static int bgcolor = 119|(154<<8)|(209<<16);
	SDL_SetRenderDrawColor(gMainRenderer, bgcolor&0xff, (bgcolor>>8)&0xff, (bgcolor>>16)&0xff, (bgcolor>>24)&0xff);
	SDL_RenderClear(gMainRenderer);
	// draw rectangle
	imgui_renderer(gMainRenderer);
	imgui_font(gMainFont);
	imgui_prepare(); {
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
		if (button(GenUIID(0),x, y+100, w, h, "color" )) 
			// this button is clicked, randomize the background color
			bgcolor = SDL_GetTicks() * 0xc3cac51a;
		// yet another button
		if (button(GenUIID(0), x+100, y+100, w, h, "quit")) { 
			// this button is clicked, quit the program
			SDL_Event ev;
			ev.type = SDL_QUIT; 
			SDL_PushEvent( &ev );
		}

		// slider bars
		x = 300; w = 22; h = 200;
		// a slider bar to tune the R channel of the background color
		slidervalue = bgcolor & 0xff; 
		if( slider(GenUIID(0), x, y, w, h, 0, 255, & slidervalue, 1)) 
			bgcolor = (bgcolor & 0xffff00) | (int)slidervalue;
		// a slider bar to tune the G channel of the background color
		slidervalue = (bgcolor >> 8) & 0xff;
		if( slider(GenUIID(0), x+50, y, w, h, 0, 255, & slidervalue, 1) )
			bgcolor = (bgcolor & 0xff00ff) | ((int)slidervalue) << 8;
		// a slider bar to tune the B channel of the background color
		slidervalue = (bgcolor >> 16) & 0xff;
		if (slider(GenUIID(0), x+100, y, w, h, 0, 255, & slidervalue, 1))
			bgcolor = (bgcolor & 0x00ffff) | ((int)(slidervalue) << 16);
		if (slider(GenUIID(0), x+100, y+h+100, h*2+w, w*2, 0, 255, & slidervalue, 1))
			bgcolor = (bgcolor & 0x00ffff) | ((int)(slidervalue) << 16);
		// a slider bar to tune the brightness of the background color
		R = bgcolor & 0xff;  G = (bgcolor >> 8) & 0xff; B = (bgcolor >> 16) & 0xff;
		slidervalue = (R + G + B)/3;
		if (slider(GenUIID(0), x+150, y, w, h, 0, 255, & slidervalue, 1)) {
			int chg = (int)slidervalue - (R + G + B)/3;
			R = CLAMP(R+chg,0,255);
			G = CLAMP(G+chg,0,255);
			B = CLAMP(B+chg,0,255);
			bgcolor = R | (G<<8) | (B<<16);
		}
		// show the value of the fgcolor
		sprintf(temp, "(%3d,%3d,%3d)", bgcolor & 0xff, (bgcolor>>8) & 0xff, (bgcolor>>16) & 0xff);
		textlabel(GenUIID(0), x, y+=h, temp);
		// a text input box
		x = 30; y += 50 ;
		if( textbox(GenUIID(0), x, y, editstring, 30) ) {
			// text is changed, you can do something here ...
		}
		textlabel( GenUIID(0), x,y+50, editstring);
	} imgui_finish();
	// present the result
	SDL_RenderPresent(gMainRenderer);
}
