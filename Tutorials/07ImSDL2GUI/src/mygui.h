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
    int mousex;     // x - mouse position 鼠标x坐标
    int mousey;     // y - mouse position 鼠标y坐标
    int mousedown;  // 1 - yes, 0 - no 是否按下鼠标

    int hotitem;    // widget below the mouse cursor 光标下的组件ID
    int activeitem; // widget under interaction 正在交互的组件ID

    int keypressed; // key that was pressed 被按下的普通键
    int keymod;     // key modifier (alt，ctrl, shift) 修饰键
    int keychar;    // char that is input 输入的字符（ASCII码）

    int lastwidget; // last widget that was processed 上一个接受键盘的组件
    int kbditem;    // widget with keyboard focus 接受键盘输入的组件ID
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
