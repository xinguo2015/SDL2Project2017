# SDL 2.0 API

## 函数 SDL_Init: 初始化SDL系统
```
int SDL_Init(Uint32 flags)
```
参数: flags是以下选项的组合（按位或操作）

| flags选项 | 子系统 |
|   -  |   -  |
| SDL_INIT_VIDEO | 视频子系统 | 
| SDL_INIT_EVENTS | 事件处理子系统 |
| SDL_INIT_TIMER | 定时器子系统 |
| SDL_INIT_AUDIO | 音频子系统 |
| SDL_INIT_JOYSTICK | 游戏杆子系统，自动包含Event子系统 | 
| SDL_INIT_HAPTIC | 触觉（力反馈）子系统|
| SDL_INIT_GAMECONTROLLER | 游戏控制子系统，自动包含Joystick子系统|
| SDL_INIT_EVERYTHING | 所有子系统 |
| SDL_INIT_NOPARACHUTE | 兼容性考虑，忽略 |


|返回值|含义|
| - | - |
| 0 | 成功|
|<0 | 失败。可调用 SDL_GetError() 获取相关信息。|

使用例程：
```
#include "SDL.h"
int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	/* ... */
	SDL_Quit();
	return 0;
}
```

	可以在需要的时候调用**SDL_InitSubSystem()**初始化所要的子系统（使用想对应的flags参数）

## 函数SDL_CreateWindow：创建窗口
```
SDL_Window* SDL_CreateWindow(const char* title,
		int         x,
		int         y,
		int         w,
		int         h,
		Uint32      flags)
```
返回值：若成功创建窗口，则返回指向窗口的指针；否则返回NULL。

| 参数 | 说明 |
| -    | -    |
| title | 窗口的标题（UTF-8 encoding）|
| x | 左上角 x - 坐标，或SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED|
| y | 左上角 y - 坐标，或SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED|
| w | 窗口的宽度（像素）|
| h | 窗口的高度（像素）|
| flags | 0, 或者 SDL_WindowFlags 的组合（按位或操作）|


```
#include "SDL.h"
#include <stdio.h>
int main(int argc, char* argv[]) 
{
	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		640,                               // width, in pixels
		480,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
		);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// The window is open: could enter program loop here (see SDL_PollEvent())

	SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
```

创建窗口flags可以是以下选项的组合（按位或操作）

| falgs选项 | 含义 |
| - | - |
| SDL_WINDOW_FULLSCREEN | 全屏窗口 |
| SDL_WINDOW_HIDDEN | 不可见窗口 |
| SDL_WINDOW_BORDERLESS | 无边框窗口 |
| SDL_WINDOW_RESIZABLE | 大小可调节窗口 |
| SDL_WINDOW_MINIMIZED | 最小化的窗口 |
| SDL_WINDOW_MAXIMIZED | 最大化的窗口 |
| SDL_WINDOW_INPUT_GRABBED | 创建后获得接受输入 |

## 函数SDL_DestroyWindow：销毁窗口
```
void SDL_DestroyWindow(SDL_Window* window)
```

## 函数SDL_CreateRenderer：创建渲染器

```
SDL_Renderer* SDL_CreateRenderer(SDL_Window* window,
	int     index,
	Uint32  flags)
```
返回值：若成功创建，则返回渲染器的指针；否则返回NULL。

| flags选项 | 含义 |
| - | - |
| window | 渲染器所对应的窗口指针 |
| index | 指定渲染器的驱动, or -1 指定第一个可用的驱动 |
| flags | 0, or 渲染器选项组合 |

使用实例：
```
#include "SDL.h"

int main(int argc, char *argv[]) 
{
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *bitmapTex = NULL;
	SDL_Surface *bitmapSurface = NULL;
	int posX = 100, posY = 100, width = 320, height = 240;

	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	bitmapSurface = SDL_LoadBMP("img/hello.bmp");
	bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
	SDL_FreeSurface(bitmapSurface);

	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(bitmapTex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();

	return 0;
}
```

| flags选项 | 含义 |
| - | - |
| SDL_RENDERER_SOFTWARE | 选择最基本软件渲染器 |
| SDL_RENDERER_ACCELERATED | 选择硬件加速的渲染器 |
| SDL_RENDERER_PRESENTVSYNC | 内容呈现与刷新率一致 |
| SDL_RENDERER_TARGETTEXTURE | 支持渲染到纹理 |
注意：如果不给出flags选项，即flags=0，那么将优先选择SDL_RENDERER_ACCELERATED。

相关函数
- SDL_CreateSoftwareRenderer
- SDL_GetRendererInfo
- SDL_DestroyRenderer
- SDL_GetNumRenderDrivers

## 函数 SDL_LoadBMP: 载入bmp格式的图像文件

```
SDL_Surface* SDL_LoadBMP(const char* file)
```
参数：file - 图像文件的文件名
返回值：如果成功载入，那么返回一个包含该图像的像面；否则返回NULL。

通过该函数创建的SDL_Surface必须通过函数SDL_FreeSurface释放：

```
void SDL_FreeSurface(SDL_Surface* surface)
```

相关函数：
	- SDL_FreeSurface
	- SDL_SaveBMP

## 函数 SDL_SaveBMP：将一个像面内容保存到一个bmp格式的文件中
```
int SDL_SaveBMP(SDL_Surface* surface,
                const char*  file)
```

## SDL常用的数据类型
- **SDL_Surface** 是一个结构，表示一个矩形形状的像素集合，用于渲染（软件方式）
成员变量。

|类型|变量名|说明|
|-|-|-|
| Uint32 | flags | 内部使用 |
| SDL_PixelFormat* | format | 像素格式（参考SDL_PixelFormat）|
| int | w, h | 矩形的宽度和高度 |
| int | pitch | 一行像素所占用的字节数 |
| void* | pixels | 存储像素的内存地址 | 
| void* | userdata | 保存用户数据的地址 | 
| int | locked | 内部使用（与锁定操作有关）| 
| void* | lock_data | 内部使用（与锁定操作有关）|
| SDL_Rect | clip_rect | 裁剪矩形（通过函数SDL_SetClipRect()指定）|
| SDL_BlitMap* | map | 内部使用（与绘制有关）
| int | refcount | 被引用的次数 | 

相关函数：
	- SDL_ConvertSurface
	- SDL_CreateRGBSurface
	- SDL_CreateRGBSurfaceFrom
	- SDL_FillRect
	- SDL_FillRects
	- SDL_FreeSurface
	- SDL_GetClipRect
	- SDL_GetColorKey
	- SDL_GetSurfaceAlphaMod
	- SDL_GetSurfaceBlendMode
	- SDL_GetSurfaceColorMod
	- SDL_LoadBMP_RW
	- SDL_LockSurface
	- SDL_LowerBlit
	- SDL_MUSTLOCK
	- SDL_SaveBMP_RW
	- SDL_SetClipRect
	- SDL_SetColorKey
	- SDL_SetSurfaceAlphaMod
	- SDL_SetSurfaceBlendMode
	- SDL_SetSurfaceColorMod
	- SDL_SetSurfacePalette
	- SDL_SetSurfaceRLE
	- SDL_SoftStretch
	- SDL_UnlockSurface
	- SDL_UpperBlit

## 函数SDL_CreateTextureFromSurface：创建纹理

```
SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer* renderer,
										  SDL_Surface*  surface)
```
| 参数名 | 说明 |
| - | - |
| renderer |  纹理所在的渲染器 |
| surface | 创建纹理所用到的像面|

返回值：如果成功，则返回所创建的纹理指针；否则返回NULL。失败时，可调用SDL_GetError获取有关信息。该函数不会改变surface所指向的像面。纹理使用完毕后，应该调用函数SDL_DestroyTexture释放掉。

代码示例：
```
Uint32 rmask, gmask, bmask, amask;
/* SDL interprets each pixel as a 32-bit number, so our masks must depend
   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

SDL_Surface *surface = SDL_CreateRGBSurface(0, 640, 480, 32, rmask, gmask, bmask, amask);

if (surface == NULL) {
	fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
	exit(1);
}

SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

if (texture == NULL) {
	fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
	exit(1);
}

SDL_FreeSurface(surface);
surface = NULL;
```

相关函数：
	- SDL_CreateTexture
	- SDL_DestroyTexture
	- SDL_QueryTexture

## 函数SDL_SetRenderDrawColor：设置渲染器的画笔颜色
```
int SDL_SetRenderDrawColor(SDL_Renderer* renderer,
		Uint8         r,
		Uint8         g,
		Uint8         b,
		Uint8         a)
```
|参数名|说明|
|-|-|
| renderer | 渲染器|
| r | 红色通道值 | 
| g | 绿色通道值 |
| b | 蓝色通道值 | 
| a | alpha通道的值 |

调用函数**SDL_SetRenderDrawBlendMode**设置alpha通道的使用规则。

使用示例：
```
SDL_Rect rectangle;
rectangle.x = 0;
rectangle.y = 0;
rectangle.w = 50;
rectangle.h = 50;
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_RenderFillRect(renderer, &rectangle);
```
相关函数：
	- SDL_GetRenderDrawColor
	- SDL_RenderClear
	- SDL_RenderDrawLine
	- SDL_RenderDrawLines
	- SDL_RenderDrawPoint
	- SDL_RenderDrawPoints
	- SDL_RenderDrawRect
	- SDL_RenderDrawRects
	- SDL_RenderFillRect
	- SDL_RenderFillRects


## 函数SDL_RenderClear：清除渲染器的内容（即清屏）。
```
int SDL_RenderClear(SDL_Renderer* renderer)
```
清屏的颜色为当前画笔的颜色（通过调用函数SDL_SetRenderDrawColor设置）

- **SDL_Rect** 表示一个矩形区域
```
typedef struct {
	int x; // 左上角的x-坐标
	int y; // 左上角的y-坐标
	int w; // 宽度
	int h; // 高度
} SDL_Rect;
```


| 类型| 名称 | 说明 |
|-    |   -  |  -   |
| int | x | the x location of the rectangle's upper left corner |
| int | y | the y location of the rectangle's upper left corner |
| int | w | the width of the rectangle |
| int | h | the height of the rectangle |


