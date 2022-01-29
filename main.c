#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
#include "myheaders/my_audio.h"
#ifdef main
#undef main
#endif

// screen dimensions
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 640
#define FPS 60

int main(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("SDL initialization failed! SDL_ERROR : %s\n", SDL_GetError());
    }

    // create a window
    SDL_Window *getUserNameWindow = SDL_CreateWindow("Login page", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *getUserNameRenderer = SDL_CreateRenderer(getUserNameWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // load background music
    load_music("music/pirates_of_caribbean.wav");

    // Start playing the background music
    SDL_PauseAudio(0);

    // load menu_login background image
    SDL_Surface * menu_login_image = SDL_LoadBMP("images/menu_login.bmp");
    SDL_Texture * menu_login_texture = SDL_CreateTextureFromSurface(getUserNameRenderer, menu_login_image);
    SDL_FreeSurface(menu_login_image); // free the surface because we don't need it anymore

    // the login window
    SDL_bool shallExit = SDL_FALSE;
    while(shallExit == SDL_FALSE){
        SDL_RenderClear(getUserNameRenderer);
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)){
            if (sdlEvent.type == SDL_QUIT){
                shallExit = SDL_TRUE;
                break;
            }
        }
        check_music_finished();
        SDL_RenderCopy(getUserNameRenderer, menu_login_texture, NULL, NULL);
        SDL_RenderPresent(getUserNameRenderer);
        SDL_Delay(1000 / FPS);
    }

    // close the login window
    SDL_DestroyWindow(getUserNameWindow);
    SDL_DestroyRenderer(getUserNameRenderer);

    // shut everything down
    SDL_CloseAudio(); // stop plating the background music
    SDL_FreeWAV(wav_buffer);
    SDL_Quit();
    return 0;
}