### 使用指南


#### 注意事项
- 不要改动此目录的任何内容
- 不要改动此目录的任何内容
- 将自己的Visual Studio工程放在目录 **Projects** 中

#### 目录说明
- Media    -- 程序运行所需的字体，图像，声音等文件
  - 建议用户将自己程序所需的字体，图像，声音等文件也放在该目录下
- SDL2Inc  -- SDL头文件所在目录
- SDL2Libs -- SDL的连接库(.lib)和运行库(.dll)文件所在的目录。
  - 建议用户将调试/运行程序的Home目录设置为该目录，从而避免改变系统的PATH环境变量


#### 建立工程的简单方法
1. 在Project中建立一个子目录，娶一个自己喜欢的名字，例如 ATest
2. 将Project中的Sample.zip解压到ATest中
3. 进入ATest目录，确认里面有文件如下：
  - CMakeList.txt
  - ImSDL2GUI.sln
  - ImSDL2GUI.vcxproj
  - ImSDL2GUI.vcxproj.user
  - src (目录)
4. 用VisualStudio打开ImSDL2GUI.sln
5. 测试是否能够正常工作：
  - 按F7进行编译/链接。
  - 按F5进行运行/调试。

#### Sample.zip中的工程路径设置的说明
工程中的路径设置采用了**相对路径**
- SDL有关的头文件(.h)的包含路径为：  ../../SDL2inc
- SDL有关的库文件(.lib)的查找路径为：../../SDL2libs
- 为了在执行/调试的时候可以找到SDL有关的DLL，已经将HOME路径设置为：../../SDL2libs 。同时运行和调试的命令行参数设置为 ..\Media, 使得程序能够找到需要的图像，字体，声音的文件。在VisualStudio进行相关的设置方法如下图：（菜单Project ==> Properties）
![调试路径设置](Media/mdpic/debugpath.png)

Sample.zip中工程ImSDL2GUI已经做了上述的设置。因此**必需**把它放在**本文件所在目录**的一个2级子目录中（例如Project/ATest/），使得VisualStudio都过上面的路径设置正确地找到SDL2inc，SDLlibs目录，从而保证VisualStudio能够正确编译和执行程序。

