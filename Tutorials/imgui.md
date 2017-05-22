# IMGUI - Immediate Mode Graphics User Interface

在传统的典型GUI中，与应用程序进行交互一般是通过**组件**（widget）完成。使用组件过程比较繁琐，包括：创建组件，显示组件，查询组件，向组件发送消息和数据，清理组件。尽管可以通过一些**所见即所得**的可视手段帮助我们完成一些步骤，但是仍然需要我们编写和维护大量代码：初始化组件，设计回调函数，清理组件。 目前绝大部分应用程序采用的是这种传统的GUI模式，称为RMGUI（Retained Mode Graphics User Interface）。

但是对于游戏类的应用程序，以及高画面刷新率的应用程序而言，传统的GUI模式显得非常繁琐。为此，xxx在2003年提出了一种新的GUI模式，即：Immediate Mode Graphics User Interface，简称IMGUI。IMGUI的使用模式如下
```
if (button(ID, posx, posy, width, height, label)) 
{
	do_something(); //该按钮被按下，执行相应任务
}
```

IMGUI的秘诀在于：button函数一次性地完成一个组件所有事情，包括：绘制组件。

