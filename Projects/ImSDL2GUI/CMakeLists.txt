cmake_minimum_required(VERSION 2.8)
project(XinGUI)
set(TestName test)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2ttf REQUIRED SDL2_ttf>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2ttf_INCLUDE_DIRS})
add_executable(${TestName} src/main.c src/mygui.c)
TARGET_LINK_LIBRARIES(${TestName} ${SDL2_LIBRARIES} ${SDL2ttf_LIBRARIES})

