#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "myheaders/my_audio.h"
#include "myheaders/my_text.h"
#include "myheaders/my_colors.h"
#include "myheaders/my_map.h"

#ifdef main
#undef main
#endif

// screen dimensions
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 640
#define FPS 20

// Unit : soldiers per second
#define RATE_OF_SOLDIERS_INCREMENT 3

int main(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SDL_Init failed!",SDL_GetError(),NULL);
    }
    if(TTF_Init() == -1){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"TTF_Init failed!",TTF_GetError(),NULL);
    }
    // open fonts
    load_fonts();

    // load background music
    load_music("music/pirates_of_caribbean.wav");

    // Start playing the background music
    SDL_PauseAudio(0);

    // ----------------------------------------------------------------------------------------------------------
    // ---------------------------------------THE START OF LOGIN PAGE--------------------------------------------
    // ----------------------------------------------------------------------------------------------------------

    // create a window
    SDL_Window *getUserNameWindow = SDL_CreateWindow("Login page", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *getUserNameRenderer = SDL_CreateRenderer(getUserNameWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

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
        SDL_Delay(1000/FPS);
    }
    SDL_StopTextInput();
    // close the login window
    SDL_DestroyTexture(menu_login_texture);
    SDL_DestroyTexture(text_box_texture);
    SDL_DestroyWindow(getUserNameWindow);
    SDL_DestroyRenderer(getUserNameRenderer);

    // ----------------------------------------------------------------------------------------------------------
    // ---------------------------------------THE END OF LOGIN PAGE----------------------------------------------
    // ----------------------------------------------------------------------------------------------------------

    // ----------------------------------------------------------------------------------------------------------
    // ---------------------------------------START OF THE MAIN GAME---------------------------------------------
    // ----------------------------------------------------------------------------------------------------------

    // create a window
    SDL_Window *mainWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // load menu_login background image
    SDL_Surface* main_background_surface = SDL_LoadBMP("images/main_background.bmp");
    SDL_Texture* main_background_texture = SDL_CreateTextureFromSurface(mainRenderer, main_background_surface);
    SDL_FreeSurface(main_background_surface); // free the surface because we don't need it anymore

    // variables
    int number_of_players = 4;
    int number_of_castles = 20;

    // generate a random map
    castle* castles = NULL;
    castles = generate_random_map(number_of_players, number_of_castles);

    int time = 0; // a variable that shows the time (value = FPS * seconds)
    is_open = SDL_TRUE;
    while(is_open){
        SDL_RenderClear(mainRenderer);
        while(SDL_PollEvent(&event)) {
            // close the window
            if (event.type == SDL_QUIT) {
                is_open = SDL_FALSE;
                break;
            }
        }
        time++;
        increment_soldiers(time, castles, FPS, RATE_OF_SOLDIERS_INCREMENT, number_of_castles);
        SDL_RenderCopy(mainRenderer, main_background_texture, NULL, NULL);
        render_map(castles, number_of_castles, mainRenderer);
        SDL_RenderPresent(mainRenderer);
        SDL_Delay(1000/FPS);
    }

    // shut everything down
    SDL_CloseAudio(); // stop plating the background music
    SDL_FreeWAV(wav_buffer);
    SDL_Quit();
    return 0;
}