# IMGUI - Immediate Mode Graphics User Interface

在传统的典型GUI中，与应用程序进行交互一般是通过**组件**（widget）完成。使用组件过程比较繁琐，包括：创建组件，显示组件，查询组件，向组件发送消息和数据，清理组件。尽管可以通过一些**所见即所得**的可视手段帮助我们完成一些步骤，但是仍然需要我们编写和维护大量代码：初始化组件，设计回调函数，清理组件。 

<div align=center>
<img src="../Media/mdpic/gui-widgets.gif" height="300" />
Windows, Mac OS X, Linux系统中一些GUI组件
</div>

标准的GUI组件库一般包含以下几种：
- text label
- button
- radio
- checkbox
- editbox
- listbox
- combolist

目前绝大部分应用程序采用的是这种传统的GUI模式，称为RMGUI（Retained Mode Graphics User Interface）。但是对于游戏类的应用程序，以及高画面刷新率的应用程序而言，传统的GUI模式显得非常繁琐。为此，xxx在2003年提出了一种新的GUI模式，即：Immediate Mode Graphics User Interface，简称IMGUI。IMGUI的核心思想是：没一个组建都由一个函数完成所有的工作，包括：维护状态，读取数据，绘制形状。

在IMGUI中，使用一个button典型方式如下：
```
if (button(ID, posx, posy, width, height, label)) 
{
	do_something(); //该按钮被按下，执行相应任务
}
```

<div align=center>
<img src="../Media/mdpic/myimgui.png" height="400" />
IMGUI组件演示
</div>

### GUI状态变量
考虑我们与计算机/应用程序进行交互的两个主要工具：鼠标和键盘。在交互过程中，我们需要知道他们的一些简单的状态信息。具体的数据结构如下：
```
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

UIState gUIState;
```

其中 **gUIState**是一个记录状态信息的全局变量。这个全局的状态变量在使用之前需要进行初始化：
```
void imgui_init()
{
	memset(&gUIState,0,sizeof(gUIState));
}
。
```
这里我们将所有的成员变量初始化为0。

在SDL程序中，记录键盘和鼠标状态的代码如下：
```
int imgui_update(SDL_Event* e)
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
```
### 按钮组件（button)
IMGUI组件库的每一种组建都是一个函数。按钮组件的函数定义如下：
```
int button(int id, int x, int y, int w, int h, char label[])
{
	SDL_Rect tr = calcTextSize(label);
	int alignDX = (w-tr.w)/2;
	int alignDY = (h-tr.h)/2;
```
|参数|说明|
|-   |-   |
| id |  组件的唯一号  |
| x, y| 组件的位置坐标（左上角） |
| w, h| 组件的宽度w和高度h       |
| label | 组件的标签字符串       |


组件的ID必须是唯一的。为了能方便的生成一个唯一的组件ID，我们定义了一个宏完成这个任务：

```
#define GenUIID(N)  ( ((__LINE__<<16) | (N & 0xFFFF))^((long)&__FILE__) )
```

其中\_\_LINE\_\_是一个特殊的宏，将被替换为代码（调用GenUIID的代码，而非定义GenUIID的代码）所在的行号；\_\_FILE\_\_也是一个特殊的宏，将被替换为代码文件名（字符串）。

宏\_\_LINE\_\_和\_\_FILE\_\_有很多用处。最常见的应用是输出程序的代码信息，便于调试查错。这里我们利用他们生成唯一号。(long)&\_\_FILE\_\_将这个字符串的地址强制转换为一个长整数。这个常整数和前面的整数((\_\_LINE\_\_<<16) | (N & 0xFFFF))进行按位的抑或运算，生成一个ID号。为了能够在同一个代码中生成多个唯一号，我们允许用户传递一个参数N，参与运算。

为了能够让按钮的标签字符串与按钮的中心对齐，button函数首先调用函数calcTextSize，计算标签字符串显示区域的矩形大小，然后确定标签的显示位置。

GUI组建是通过鼠标进行交互的。当鼠标进入组建的区域中，组建变成一个热点组件。如果用户还按下了鼠标按钮，那么热点组件成为当前活跃组件。
```
	// Check whether the button should be hot
	if (regionhit(x, y, w, h))
	{
		gUIState.hotitem = id;
		if (gUIState.activeitem == 0 && gUIState.mousedown)
			gUIState.activeitem = id;
	}
```
函数 **regionhit** 判断当前的鼠标位置是否位于矩形(x, y, w, h)之中。
```
int regionhit(int x, int y, int w, int h)
{
	return (gUIState.mousex > x && gUIState.mousey > y &&
		gUIState.mousex < x + w && gUIState.mousey < y + h);
}
```
按钮的组件函数接下来根据组件的状态绘制按钮。
```
	// Draw button 
	if (gUIState.hotitem == id)	{
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
```

最后，根据判断：**按钮是否被按下然后释放**。如果是，返回1；否则返回0。注意：按钮被按下，并不会使得函数返回1。只有完成完整的click and release按钮过程，才会返回1。
```
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
```
至此，我们已经完成了一个简单的button组件。它能够接受鼠标的操作。我们没有让button处理键盘的操作。对于处在光标下的button来讲，通常按下和释放空格键，与按下和释放鼠标按钮能够起到相同的作用。你们可以对上述程序进行扩展，实现该功能。

###GUI状态调整
每次执行组件之前和之后，我们需要对GUI的状态进行一些事前和事后的处理工作，防止状态混乱。在执行组件之前，我们需要调用下面的函数，进行一个必要的准备工作：
```
void imgui_prepare()
{
		gUIState.hotitem = 0;
}
```
执行完所有的组件之后，需要进行一些事后清理工作：
```
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
```
### 滑动条组件（slider)
### 滑动条组件（slider)
### 滑动条组件（slider)
### 滑动条组件（slider)
### 滑动条组件（slider)
