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
// So, if you call GenUIID(0) in a while/for loop body, the same ID is
// obtained. In this case, you need call GenUIID with different N parameter: 
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

enum
{
	UIMaxLabel = 64,
	UIMaxText = 256,
};

typedef struct 
{
	SDL_Rect mRect;
	char     mLabel[UIMaxLabel];
	char     mText[UIMaxText];
	int      mKBFocusHint;
}
UIParam;

void imgui_init();
void imgui_renderer(SDL_Renderer* ren);
void imgui_font(TTF_Font* font);
void imgui_prepare();
void imgui_finish();

int button(int id, int x, int y, int w, int h, char label[]);
int slider(int id, int x, int y, int w, int h, double minv, double maxv, double * value, double delta);
int textlabel(int id, int x, int y, char text[]);
int textbox(int id, int x, int y, char textbuf[], int maxbuf);

// style and customize settting
void setButtonClickMovement(int m); // movement when clicked

extern UIState uistate;

#endif // ifndef ___simple_gui_sdl2_IMGUI__________________
