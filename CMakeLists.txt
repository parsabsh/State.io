cmake_minimum_required(VERSION 3.21)
project(project_parsabsh C)

set(CMAKE_C_STANDARD 11)

set(SDL2_INCLUDE_DIR C:/MinGW/include)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(project_parsabsh main.c myheaders/my_audio.c myheaders/my_text.c myheaders/my_colors.c myheaders/my_map.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf -mwindows)