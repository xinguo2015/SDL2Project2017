#include "mygui.h"

#ifndef CLAMP
#define CLAMP(x,min,max) ((x)<(min) ? (min) : ((x)>(max)?(max):(x)) )
#endif

#define SPLIT_COLOR(c) ((c)&0xFF), (((c)>>8)&0xFF), (((c)>>16)&0xFF), (((c)>>24)&0xFF)

static int guiColorStill  = 0xCCBBAA;
static int guiColorHot    = 0xFFDDCC;
static int guiColorCheck  = 0xFFEEDD;
static int guiColorWhite  = 0xFFFFFF;
static int guiColorFocus  = 0xFFEEDD;
static int guiColorEdit   = 0xFFFFFF;
static int guiColorPicked = 0xFFEEDD;
//static int guiColorLabel  = 0xFFFF00;
static int guiColorLabel  = 0x333333;

static SDL_Renderer * guiRenderer = NULL;
static TTF_Font *     guiFont = NULL;
static int            guiFontHeight;
static int            guiButtonSink = 2;
static int            guiItemHeight = 30;

UIState gUIState;

void imgui_renderer(SDL_Renderer* ren)
{
	guiRenderer = ren;
}

void imgui_font(TTF_Font* font)
{
	guiFont = font;
	guiFontHeight = TTF_FontHeight(font);	
}

void setButtonSink(int m)
{
	guiButtonSink = m;
}


void imgui_init()
{
	memset(&gUIState,0,sizeof(gUIState));
}

void imgui_prepare()
{
	gUIState.hotitem = 0;
}

void imgui_finish()
{
	if (gUIState.mousedown == 0)
	{
		gUIState.activeitem = 0;
	}
	// If no widget grabbed tab, clear focus
	if (gUIState.keypressed == SDLK_TAB)
		gUIState.kbditem = 0;
	// Clear the entered key
	gUIState.keypressed = 0;  
	gUIState.keychar = 0;
}

int imgui_handle(SDL_Event* e)
{
    switch (e->type) {
        case SDL_MOUSEMOTION:
            // update mouse position
            gUIState.mousex = e->motion.x;
            gUIState.mousey = e->motion.y;
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            // update button down state
            if (e->button.button == 1) { //这里只处理鼠标**左**键
                gUIState.mousedown = 1;
                return 1;
            }
            return 0;
        case SDL_MOUSEBUTTONUP:
            // update button down state if left-clicking
            if (e->button.button == 1) { //这里只处理鼠标**左**键
                gUIState.mousedown = 0;
                return 1;
            }
            return 0;
        case SDL_KEYDOWN:
            // a key is pressed
            gUIState.keypressed = e->key.keysym.sym;
            gUIState.keymod = e->key.keysym.mod;
            return 1;
        case SDL_TEXTINPUT:
            // a character is input
            gUIState.keychar = e->text.text[0];
            return 1;
    }
    return 0;
}

SDL_Color makecolor(int color)
{
	SDL_Color cc = { SPLIT_COLOR(color) };
	return cc;
}

SDL_Rect drawstring(char s[], int x, int y, int color)
{
	SDL_Rect rect = { x,y,0,0 };
	SDL_Surface* surf;
	SDL_Texture* tex;

	surf = TTF_RenderText_Blended(guiFont, s, makecolor(color));
	if( !surf ) return rect;

	tex = SDL_CreateTextureFromSurface(guiRenderer,surf);
	SDL_FreeSurface(surf); //释放surface
	if( !tex ) return rect;

	SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
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
	SDL_RenderDrawRect(guiRenderer, &r);
}

void fillrect(int x, int y, int w, int h, int color)
{
	SDL_Rect r = {x,y,w,h};
	SDL_SetRenderDrawColor(guiRenderer, SPLIT_COLOR(color));
	SDL_RenderFillRect(guiRenderer, &r);
}

// Check whether current mouse position is within a rectangle
int regionhit(int x, int y, int w, int h)
{
	return (gUIState.mousex > x && gUIState.mousey > y &&
		gUIState.mousex < x + w && gUIState.mousey < y + h);
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
		gUIState.hotitem = id;
		if (gUIState.activeitem == 0 && gUIState.mousedown)
			gUIState.activeitem = id;
	}
	// Draw button 
    if (gUIState.hotitem == id)    {
        // button is hot
        if ( ! gUIState.mousedown ) {
            // Button is merely 'hot', 绘制颜色为guiColorHot
            fillrect(x, y, w, h, guiColorHot);
            // 添加按钮的标签
            drawstring(label, x+alignDX, y+alignDY,guiColorLabel);
        } else {
            // mouse is down，将按钮的位置进行稍许偏移，增加动感
            fillrect(x+guiButtonSink, y+guiButtonSink, w, h, guiColorHot);
            // 添加按钮的标签
            drawstring(label, x+guiButtonSink+alignDX, y+guiButtonSink+alignDY, guiColorLabel);
        }
    } else {
        // button is not hot
        fillrect(x, y, w, h, guiColorStill);
        drawstring(label, x+alignDX, y+alignDY, guiColorLabel);
    }

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gUIState.hotitem == id && 
		gUIState.activeitem == id &&
		gUIState.mousedown == 0 )
	{
		return 1;
	}

	// Otherwise, no clicky (click and release).
	return 0;
}

int checkbox (int id, int x, int y, int w, int h, char label[], int *value)
{
	// Check whether the button should be hot
	if (regionhit(x, y, w, h))
	{
		gUIState.hotitem = id;
		if (gUIState.activeitem == 0 && gUIState.mousedown)
			gUIState.activeitem = id;
	}
	// Draw radio button
	fillrect(x,y, w, h, gUIState.hotitem == id ? guiColorHot : guiColorStill);
	drawrect(x,y, w, h, guiColorCheck);
	drawstring(label, x+w+4, y-10, guiColorLabel);
	if ( *value )
	{	// is checked, then draw a cross
		SDL_SetRenderDrawColor(guiRenderer, SPLIT_COLOR(guiColorCheck));
		// draw the cross 
		SDL_RenderDrawLine(guiRenderer, x,y, x+w-1, y+h-1);
		SDL_RenderDrawLine(guiRenderer, x+1,y, x+w-1, y+h-2);
		SDL_RenderDrawLine(guiRenderer, x,y+1, x+w-2, y+h-1);
		SDL_RenderDrawLine(guiRenderer, x,y+h-1, x+w-1, y);
		SDL_RenderDrawLine(guiRenderer, x+1,y+h-1, x+w-1, y+1);
		SDL_RenderDrawLine(guiRenderer, x,y+h-2, x+w-2, y);
	}

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gUIState.hotitem == id && 
		gUIState.activeitem == id &&
		gUIState.mousedown == 0 )
	{
		*value = !(*value);
		return 1;
	}

	return 0;
}


int radio(int id, int x, int y, int w, int h, char label[], int reference, int *value)
{
	// Check whether the button should be hot
	if (regionhit(x, y, w, h))
	{
		gUIState.hotitem = id;
		if (gUIState.activeitem == 0 && gUIState.mousedown)
			gUIState.activeitem = id;
	}
	// Draw radio button
	fillrect(x,y, w, h, gUIState.hotitem == id ? guiColorHot : guiColorStill);
	drawstring(label, x+w+4, y-10, guiColorLabel);
	//drawrect(x,y, w, h, guiColorCheck);
	if ( reference == *value )
	{
		SDL_SetRenderDrawColor(guiRenderer, SPLIT_COLOR(0));
		fillrect(x+w/4,y+h/4, w/2, h/2, guiColorCheck);
	}

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gUIState.hotitem == id && 
		gUIState.activeitem == id &&
		gUIState.mousedown == 0 )
	{
		if( *value != reference )
		{
			*value = reference;
			return 1;
		}
	}

	return 0;
}



// Simple scroll bar IMGUI widget
int slider(int id, int x, int y, int w, int h, double min, double max, double delta, double * value)
{
	int cursize = 16; // cursor size
	int border = 2;   // distance against the cursor
	int hintsize = 2; // thickness of the focus hint
	int vertical = w < h;// sliding direction
	double posratio = CLAMP((*value - min)/(max-min), 0, 1);
	int curpos;

	w = CLAMP( w, cursize+border*2, w);
	h = CLAMP( h, cursize+border*2, h);
	if( vertical ) h = h<cursize*4 ? cursize*4 : h;
	else           w = w<cursize*4 ? cursize*4 : w;

	curpos = (int)( posratio * ((vertical?h:w) - border*2 - cursize) ) + border;
	
	// Check for hotness
	if (regionhit(x, y, w, h)) {
		gUIState.hotitem = id;
		if (gUIState.activeitem == 0 && gUIState.mousedown)
			gUIState.activeitem = id;
	}

	// If no widget has keyboard focus, take it
	if (gUIState.kbditem == 0)
		gUIState.kbditem = id;

	// If we have keyboard focus, show it
	if (gUIState.kbditem == id)
		drawrect(x-hintsize, y-hintsize, w+hintsize*2, h+hintsize*2, guiColorFocus);

	// render the bar
	fillrect(x, y, w, h, guiColorStill   );
	// render the cursor
	if (gUIState.activeitem == id || gUIState.hotitem == id) {
		fillrect( vertical ? x+(w-cursize)/2 : x+curpos,
			vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorWhite);
	} else {
		fillrect( vertical ? x+(w-cursize)/2 : x+curpos,
			vertical ? y + curpos : y+(h-cursize)/2, cursize, cursize, guiColorHot);
	}
	// If we have keyboard focus, we'll need to process the keys
	if (gUIState.kbditem == id)
	{
		switch (gUIState.keypressed)
		{
		case SDLK_TAB:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			gUIState.kbditem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (gUIState.keymod & KMOD_SHIFT)
				gUIState.kbditem = gUIState.lastwidget;
			// Also clear the key so that next widget
			// won't process it
			gUIState.keypressed = 0;
			break;
		case SDLK_UP:
		case SDLK_LEFT:
			// Slide slider up (if not at zero)
			if (*value > 0) {
				(*value) = CLAMP(*value - delta, 0, max);
				return 1;
			}
			break;
		case SDLK_DOWN:
		case SDLK_RIGHT:
			// Slide slider down (if not at max)
			if (*value < max) {
				(*value) = CLAMP(*value + delta, 0, max);
				return 1;
			}
			break;
		}
	}

	gUIState.lastwidget = id;

	// Update widget value
	if (gUIState.activeitem == id) {
		double newvalue = vertical ? 
			(gUIState.mousey - (y + border + cursize/2))/(double)(h-border*2-cursize) :
			(gUIState.mousex - (x + border + cursize/2))/(double)(w-border*2-cursize) ;
		newvalue = min + CLAMP(newvalue,0,1)*(max-min);
		gUIState.kbditem = id; // let it accept keyboard
		if (*value != newvalue ) {
			*value = newvalue;
			return 1;
		}
	}

	return 0;
}

int textbox(int id, int x, int y, int w, int h, char textbuf[], int maxbuf)
{
	int len = strlen(textbuf);
	int cursorpos = 0;
	int textChanged = 0;
	// Check whether the button should be hot
	if (regionhit(x, y, w, h))
	{
		gUIState.hotitem = id;
		if (gUIState.activeitem == 0 && gUIState.mousedown)
			gUIState.activeitem = id;
	}
	// If no widget has keyboard focus, take it
	if (gUIState.kbditem == 0)
		gUIState.kbditem = id;
	// If we have keyboard focus, show it
	if (gUIState.kbditem == id)
		drawrect(x-2, y-2, w+4, h+4, 0xffddee);

	// Render the text box 
	if ( gUIState.hotitem == id || gUIState.activeitem == id ) {
		// 'hot' or 'active'
		fillrect(x, y, w, h, guiColorHot);
	} else {
		fillrect(x, y, w, h, guiColorStill   );
	}

	// show text
	cursorpos = x+4 + drawstring(textbuf, x+4, y-6, guiColorEdit).w;
	// Render cursor if we have keyboard focus
	if ( gUIState.kbditem == id && (SDL_GetTicks() >> 8) & 1)
		drawstring("_", cursorpos, y-6, guiColorEdit);

	// If we have keyboard focus, we'll need to process the keys
	if (gUIState.kbditem == id)
	{
		switch (gUIState.keypressed)
		{
		case SDLK_TAB:
		case SDLK_RETURN:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			gUIState.kbditem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (gUIState.keypressed==SDLK_TAB && gUIState.keymod & KMOD_SHIFT)
				gUIState.kbditem = gUIState.lastwidget;
			// Also clear the key so that next widget
			// won't process it
			gUIState.keypressed = 0;
			break;
		case SDLK_BACKSPACE:
			if( len > 0 ) {
				textbuf[--len] = 0;
				textChanged = 1;
			}
			gUIState.keypressed = 0;
			break;
		}
		if (gUIState.keychar >= 32 && gUIState.keychar < 127 && len < maxbuf ) {
			textbuf[len] = gUIState.keychar;
			textbuf[++len] = 0;
			textChanged = 1;
		}
	}

	gUIState.lastwidget = id;

	// If button is hot and active, but mouse button is not
	// down, the user must have clicked the button.
	if (   gUIState.mousedown == 0 
		&& gUIState.hotitem == id 
		&& gUIState.activeitem == id )
		gUIState.kbditem = id;
	return textChanged;
}

int textlabel(int id, int x, int y, char text[])
{
	drawstring(text, x, y, guiColorLabel);
	return 0;
}


static int listitem(int id, int x, int y, int w, int h, char label[], int selected)
{
	if (regionhit(x, y, w, h))
	{
		gUIState.hotitem = id;
		if (gUIState.activeitem == 0 && gUIState.mousedown)
			gUIState.activeitem = id;
	}

	if (gUIState.hotitem == id	&& gUIState.mousedown )
		selected = 1;

	if (gUIState.hotitem == id)
		fillrect(x, y, w, h, guiColorHot);
	else if ( selected )
		fillrect(x, y, w, h, guiColorPicked);
	else
		fillrect(x, y, w, h, guiColorStill);
	drawstring(label, x+5, y-10, guiColorLabel);

	// If button is hot and active, but mouse button is not down, 
	// the user must have clicked the button.
	if (gUIState.hotitem == id && 
		gUIState.activeitem == id &&
		gUIState.mousedown == 0 )
	{
		return 1;
	}

	// Otherwise, no clicky.
	return 0;
}

int listbox (int id, int x, int y, int w, int h, char*items[], int nitem, int *firstitem, int *selection)
{
	int needslider = 0;
	double slidervalue = *firstitem;
	int nShow, k, wext;
	int newSelection = *selection;
	
	
	nShow = CLAMP( (h-4) / guiItemHeight, 1, nitem); 
	if( nShow<nitem ) needslider = 1;

	fillrect(x,y,w,h,guiColorStill);
	if( needslider && slider(id+GenUIID(0), x+w-2, y+2, 20, h-4, (double)0, (double)(nitem-nShow+1), 1.0, &slidervalue) ) {
		*firstitem = (int)(slidervalue+0.1);
	}
	
	wext = nShow<nitem ? w + 20 : w;
	drawrect(x,  y,  wext,  h,  0x77777777);
	drawrect(x+1,y+1,wext-2,h-2,0x77777777);
	
	for( k = 0; k<nShow; k++ ) {
		int iid = k + *firstitem;
		if( iid<nitem && listitem(id+GenUIID(k), x+2, y+2+k*guiItemHeight, w-4, guiItemHeight, items[iid], iid==*selection) )
			newSelection = iid;
	}

	if( needslider )
		drawrect(x+w-4, y, 2, h, 0x77777777);
	
	if( *selection != newSelection ) {
		*selection = newSelection	;
		return 1;
	}
	return 0;
}