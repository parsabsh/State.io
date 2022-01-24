//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#ifdef main
#undef main
#endif

//Screen dimension constants
const int FPS = 60;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("SDL initialization failed! SDL_ERROR : %s\n", SDL_GetError());
    }
    SDL_Window *getUserNameWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                             SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Delay(3000);
    SDL_Quit();
    return 0;
}