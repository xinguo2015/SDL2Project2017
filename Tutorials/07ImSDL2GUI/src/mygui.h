/* header file for a simple GUI */
#ifndef ___simple_gui_sdl2_IMGUI__________________
#define ___simple_gui_sdl2_IMGUI__________________

#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"

//===========================================================================
//
//   [L:16-1][N:16-1]
//        XOR
//   [F:32 --------1]
// 
// Generate a *fake* unique ID for gui controls at compiling/run time
//
#define GenUIID(N) ( ((__LINE__<<16) | ( N & 0xFFFF))^((long)&__FILE__) )
//
// GenUIID(0) will give a unique ID at each source code line. 
// If you need one UI ID per line, just call GenUIID with 0
//
//               GenUIID(0)
//
// But, two GenUIID(0) calls at the same line will give the same ID.
// So, in a while/for loop body, GenUIID(0) will give you the same ID.
// In this case, you need call GenUIID with different N parameter: 
//
//               GenUIID(N)
//
//===========================================================================

typedef struct 
{
	int mousex;     // x - mouse position
	int mousey;     // y - mouse position
	int mousedown;  // 1 - yes, 0 - no

	int hotitem;    // widget below the mouse cursor
	int activeitem; // widget under interaction 

	int kbditem;    // widget that has the keyboard focus
	int keypressed; // key that was pressed
	int keymod;     // key modifier flags (such as shift pressed)
	int keychar;    // char that is input
	int lastwidget; // last widget that was processed
	
}
UIState;


void imgui_init();
void imgui_renderer(SDL_Renderer* ren);
void imgui_font(TTF_Font* font);
void imgui_prepare();
int  imgui_update(SDL_Event* e);
void imgui_finish();

int button   (int id, int x, int y, int w, int h, char label[]);
int checkbox (int id, int x, int y, int w, int h, char label[], int *value);
int radio    (int id, int x, int y, int w, int h, char label[], int reference, int *value);
int listbox  (int id, int x, int y, int w, int h, char*items[], int nitem, int *liststart, int *value);
int slider   (int id, int x, int y, int w, int h, double minv, double maxv, double delta, double * value);
int textbox  (int id, int x, int y, int w, int h, char textbuf[], int maxbuf);
int textlabel(int id, int x, int y, char text[]);

extern UIState gUIState;

#endif // ifndef ___simple_gui_sdl2_IMGUI__________________
