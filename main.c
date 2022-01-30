#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>
#include "myheaders/my_audio.h"
#include "myheaders/my_text.h"
#include "myheaders/my_colors.h"

#ifdef main
#undef main
#endif

// screen dimensions
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 640
#define FPS 60

int main(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL_Init failed!",SDL_GetError(),NULL);
    }
    if(TTF_Init() == -1){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"TTF_Init failed!",TTF_GetError(),NULL);
    }
    // open fonts
    load_fonts();

    // create a window
    SDL_Window *getUserNameWindow = SDL_CreateWindow("Login page", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *getUserNameRenderer = SDL_CreateRenderer(getUserNameWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // load background music
    load_music("music/pirates_of_caribbean.wav");

    // Start playing the background music
    SDL_PauseAudio(0);

    // load menu_login background image
    SDL_Surface* surface = SDL_LoadBMP("images/login_background.bmp");
    SDL_Texture* menu_login_texture = SDL_CreateTextureFromSurface(getUserNameRenderer, surface);
    SDL_FreeSurface(surface); // free the surface because we don't need it anymore

    // load text box for input username
    SDL_Surface* surface2 = SDL_LoadBMP("images/text_box_simple.bmp");
    SDL_Texture* text_box_texture = SDL_CreateTextureFromSurface(getUserNameRenderer, surface2);
    SDL_FreeSurface(surface2); // free the surface because we don't need it anymore
    SDL_Rect text_box_rect = {390, 280, 300, 80};

    // username input variables
    SDL_Rect userNameInput_rect = {530, 300, 20, 40};
    SDL_SetTextInputRect(&userNameInput_rect);
    char user_name[14] = "";
    SDL_Event event;
    SDL_StartTextInput();

    // the login window
    SDL_bool is_open = SDL_TRUE;
    while(is_open){
        SDL_RenderClear(getUserNameRenderer);

        while(SDL_PollEvent(&event)){
            // close the window
            if(event.type == SDL_QUIT){
                is_open = SDL_FALSE;
                break;
            }
            if(text_input(event, user_name, &userNameInput_rect, getUserNameWindow, 12) == 0){
                is_open = SDL_FALSE;
                break;
            }
        }
        check_music_finished();
        SDL_RenderCopy(getUserNameRenderer, menu_login_texture, NULL, NULL);
        SDL_RenderCopy(getUserNameRenderer, text_box_texture, NULL, &text_box_rect);
        print_text(consola_font, user_name, CASTLE_OUTLINE_COLOR, getUserNameRenderer, &userNameInput_rect);
        SDL_RenderPresent(getUserNameRenderer);
        SDL_Delay(1000/20);
    }

    SDL_StopTextInput();
    // close the login window
    SDL_DestroyWindow(getUserNameWindow);
    SDL_DestroyRenderer(getUserNameRenderer);

    // shut everything down
    SDL_CloseAudio(); // stop plating the background music
    SDL_FreeWAV(wav_buffer);
    SDL_Quit();
    return 0;
}