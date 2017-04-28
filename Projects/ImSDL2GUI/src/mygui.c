#include "mygui.h"

#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

#define SPLIT_COLOR(c) ((c)&0xFF), (((c)>>8)&0xFF), (((c)>>16)&0xFF), (((c)>>24)&0xFF)

static int guiColorBlack  = 0;
static int guiColorDark   = 0x443322;
static int guiColorGray   = 0xCCBBAA;
static int guiColorBright = 0xFFDDCC;
static int guiColorWhite  = 0xFFFFFF;
static int guiColorFocus  = 0xFFEEDD;
static int guiColorLabel  = 0xFFFF00;
static int guiColorEdit   = 0xFF00FF;

static SDL_Renderer * guiRenderer = NULL;
static TTF_Font *     guiFont = NULL;
static int            guiFontHeight;
static int            guiButtonMovement = 2;

UIState uistate;


void imgui_renderer(SDL_Renderer* ren)
{
	guiRenderer = ren;
}

void imgui_font(TTF_Font* font)
{
	guiFont = font;
	guiFontHeight = TTF_FontHeight(font);	
}

void setButtonClickMovement(int m)
{
	guiButtonMovement = m;
}


void imgui_init()
{
	memset(&uistate,0,sizeof(uistate));
}

void imgui_prepare()
{
	uistate.hotitem = 0;
}

void imgui_finish()
{
	if (uistate.mousedown == 0)
	{
		uistate.activeitem = 0;
	}
	else
	{
		if (uistate.activeitem == 0)
			uistate.activeitem = -1;
	}
	// If no widget grabbed tab, clear focus
	if (uistate.keypressed == SDLK_TAB)
		uistate.kbditem = 0;
	// Clear the entered key
	uistate.keypressed = 0;  
	uistate.keychar = 0;
}

SDL_Color makecolor(int color)
{
	SDL_Color cc = { SPLIT_COLOR(color) };
	return cc;
}

SDL_Rect drawstring(char s[], int x, int y, int color)
{
	SDL_Rect rect = { 0,0,0,0 };
	SDL_Surface* surf;
	SDL_Texture* tex;

	surf = TTF_RenderText_Blended(guiFont, s, makecolor(color));
	if( !surf ) return rect;

	tex = SDL_CreateTextureFromSurface(guiRenderer,surf);
	SDL_FreeSurface(surf); //ÊÍ·Åsurface
	if( !tex ) return rect;

	rect.x = x; rect.y = y;
	SDL_QueryTexture(tex,NULL,NULL, &rect.w, &rect.h);
	SDL_RenderCopy(guiRenderer, tex, NULL, &rect);
	SDL_DestroyTexture(tex);
	return rect;
}

SDL_Rect calcTextSize(char t[])
{
	SDL_Rect rt; rt.x = rt.y = 0;
	TTF_SizeText(guiFont, t, &rt.w, &rt.h);
	return rt;
}

void drawrect(int x, int y, int w, int h, int color)
{
	SDL_Rect r = {x,y,w,h};
	SDL_SetRenderDrawColor(guiRenderer, SPLIT_COLOR(color));
	SDL_RenderFillRect(guiRenderer, &r);
}

// Check whether current mouse position is within a rectangle
int regionhit(int x, int y, int w, int h)
{
	return (uistate.mousex > x && uistate.mousey > y &&
		uistate.mousex < x + w && uistate.mousey < y + h);
}
// Simple button IMGUI widget
int button(int id, int x, int y, int w, int h, char label[])
{
	SDL_Rect tr = calcTextSize(label);
	int alignDX = (w-tr.w)/2;
	int alignDY = (h-tr.h)/2;
	// Check whether the button should be hot
	if (regionhit(x, y, w, h))
	{
		uistate.hotitem = id;
		if (uistate.activeitem == 0 && uistate.mousedown)
			uistate.activeitem = id;
	}

	// Render button 
	if (uistate.hotitem == id)	{
		if (uistate.activeitem == id) {
			// Button is both 'hot' and 'active'
			drawrect(x+guiButtonMovement, y+guiButtonMovement, w, h, guiColorBright);
			drawstring(label, x+guiButtonMovement+alignDX, y+guiButtonMovement+alignDY, guiColorLabel);
		} else {
			// Button is merely 'hot'
			drawrect(x, y, w, h, guiColorBright);
			//guiFontHeight
			drawstring(label, x+alignDX, y+alignDY,guiColorLabel);
		}
	} else {
		// button is not hot, but it may be active    
		drawrect(x, y, w, h, guiColorGray);
		drawstring(label, x+alignDX, y+alignDY, guiColorLabel);
	}

	// If button is hot and active, but mouse button is not
	// down, the user must have clicked the button.
	if (uistate.mousedown == 0 && uistate.hotitem == id && uistate.activeitem == id)
		return 1;

	// Otherwise, no clicky.
	return 0;
}

// Simple scroll bar IMGUI widget
int slider(int id, int x, int y, int w, int h, double min, double max, double * value, double delta)
{
	int cursize = 16; // cursor size
	int border = 2;   // distance against the cursor
	int hintsize = 2; // thickness of the focus hint
	int vertical = w < h;
	double v = (*value - min)/(max-min);
	int curpos;

	if( w<cursize+border*2 ) w = cursize + border*2;
	if( h<cursize+border*2 ) h = cursize + border*2;
	if( vertical ) h = h<cursize*4 ? cursize*4 : h;
	else           w = w<cursize*4 ? cursize*4 : w;

	curpos = (int)( CLAMP(v,0,1) * ((vertical?h:w)-border*2-cursize) ) + border;
	// Check for hotness
	if (regionhit(x, y, w, h)) {
		uistate.hotitem = id;
		if (uistate.activeitem == 0 && uistate.mousedown)
			uistate.activeitem = id;
	}

	// If no widget has keyboard focus, take it
	if (uistate.kbditem == 0)
		uistate.kbditem = id;

	// If we have keyboard focus, show it
	if (uistate.kbditem == id)
		drawrect(x-hintsize, y-hintsize, w+hintsize*2, h+hintsize*2, guiColorFocus);

	// render the bar
	drawrect(x, y, w, h, guiColorGray   );
	// render the cursor
	if (uistate.activeitem == id || uistate.hotitem == id) {
		drawrect( vertical ? x+(w-cursize)/2 : x+curpos,
			vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorWhite);
	} else {
		drawrect( vertical ? x+(w-cursize)/2 : x+curpos,
			vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorBright);
	}
	// If we have keyboard focus, we'll need to process the keys
	if (uistate.kbditem == id)
	{
		switch (uistate.keypressed)
		{
		case SDLK_TAB:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			uistate.kbditem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (uistate.keymod & KMOD_SHIFT)
				uistate.kbditem = uistate.lastwidget;
			// Also clear the key so that next widget
			// won't process it
			uistate.keypressed = 0;
			break;
		case SDLK_UP:
			// Slide slider up (if not at zero)
			if (*value > 0) {
				(*value) -= delta;
				return 1;
			}
			break;
		case SDLK_DOWN:
			// Slide slider down (if not at max)
			if (*value < max) {
				(*value) += delta;
				return 1;
			}
			break;
		}
	}

	uistate.lastwidget = id;

	// Update widget value
	if (uistate.activeitem == id) {
		uistate.kbditem = id;
		v = vertical ? (uistate.mousey - (y + border + cursize/2))/(double)(h-border*2-cursize)
			:          (uistate.mousex - (x + border + cursize/2))/(double)(w-border*2-cursize);
		v = min + CLAMP(v,0,1)*(max-min);
		if ( v != *value) {
			*value = v;
			return 1;
		}
	}

	return 0;
}

int textbox(int id, int x, int y, char textbuf[], int maxbuf)
{
	int w = maxbuf*16, h = 32;
	int len = strlen(textbuf);
	int cursorpos = 0;
	int textChanged = 0;
	// Check whether the button should be hot
	if (regionhit(x-4, y-4, w+8, h+8))
	{
		uistate.hotitem = id;
		if (uistate.activeitem == 0 && uistate.mousedown)
			uistate.activeitem = id;
	}
	// If no widget has keyboard focus, take it
	if (uistate.kbditem == 0)
		uistate.kbditem = id;
	// If we have keyboard focus, show it
	if (uistate.kbditem == id)
		drawrect(x-6, y-6, w+12, h+12, 0xffddee);

	// Render the text box 
	if ( uistate.hotitem == id || uistate.activeitem == id ) {
		// 'hot' or 'active'
		drawrect(x-4, y-4,w+8, h+8, guiColorBright);
	} else {
		drawrect(x-4, y-4,w+8, h+8, guiColorGray   );
	}

	// show text
	cursorpos = x + drawstring(textbuf,x,y, guiColorEdit).w;
	// Render cursor if we have keyboard focus
	if ( uistate.kbditem == id && (SDL_GetTicks() >> 8) & 1)
		drawstring("_", cursorpos, y, guiColorEdit);

	// If we have keyboard focus, we'll need to process the keys
	if (uistate.kbditem == id)
	{
		switch (uistate.keypressed)
		{
		case SDLK_TAB:
		case SDLK_RETURN:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			uistate.kbditem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (uistate.keypressed==SDLK_TAB && uistate.keymod & KMOD_SHIFT)
				uistate.kbditem = uistate.lastwidget;
			// Also clear the key so that next widget
			// won't process it
			uistate.keypressed = 0;
			break;
		case SDLK_BACKSPACE:
			if( len > 0 ) {
				textbuf[--len] = 0;
				textChanged = 1;
			}
			break;
		}
		if (uistate.keychar >= 32 && uistate.keychar < 127 && len < maxbuf ) {
			textbuf[len] = uistate.keychar;
			textbuf[++len] = 0;
			textChanged = 1;
		}
	}

	uistate.lastwidget = id;

	// If button is hot and active, but mouse button is not
	// down, the user must have clicked the button.
	if (   uistate.mousedown == 0 
		&& uistate.hotitem == id 
		&& uistate.activeitem == id )
		uistate.kbditem = id;
	return textChanged;
}

int textlabel(int id, int x, int y, char text[])
{
	drawstring(text, x, y, guiColorLabel);
	return 0;
}