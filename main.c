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
    SDL_Surface* surface1 = SDL_LoadBMP("images/login_background.bmp");
    SDL_Texture* menu_login_texture = SDL_CreateTextureFromSurface(getUserNameRenderer, surface1);
    SDL_FreeSurface(surface1); // free the surface because we don't need it anymore
    surface1 = NULL;

    // load text box for input username
    SDL_Surface* surface2 = SDL_LoadBMP("images/text_box_simple.bmp");
    SDL_Texture* text_box_texture = SDL_CreateTextureFromSurface(getUserNameRenderer, surface2);
    SDL_FreeSurface(surface2); // free the surface because we don't need it anymore
    SDL_Rect text_box_rect = {390, 280, 300, 80};
    surface2 = NULL;

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
    // ---------------------------------START OF THE MAIN MENU + MAIN GAME---------------------------------------
    // ----------------------------------------------------------------------------------------------------------

    // create a window
    SDL_Window *mainWindow = SDL_CreateWindow("State.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // load menu background image
    SDL_Surface* surface3 = SDL_LoadBMP("images/menu_background.bmp");
    SDL_Texture* menu_texture = SDL_CreateTextureFromSurface(mainRenderer, surface3);
    SDL_FreeSurface(surface3); // free the surface because we don't need it anymore

    // load main game background image
    SDL_Surface* main_background_surface = SDL_LoadBMP("images/main_background.bmp");
    SDL_Texture* main_background_texture = SDL_CreateTextureFromSurface(mainRenderer, main_background_surface);
    SDL_FreeSurface(main_background_surface); // free the surface because we don't need it anymore

    SDL_Rect quick_rect;
    SDL_Rect scoreboard_rect;
    SDL_Rect quit_rect;
    evaluate_menu_rects(&quick_rect, &scoreboard_rect, &quit_rect);

    while(1){
        // variables
        int number_of_players;
        int number_of_castles;
        int points_of_players[4][2] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
        castle* castles = show_menu(mainRenderer, &quick_rect, &scoreboard_rect, &quit_rect, menu_texture, &number_of_players, &number_of_castles, points_of_players, user_name);
        if(castles == NULL){
            break;
        }
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
            check_music_finished();
            increment_soldiers(time, castles, FPS, RATE_OF_SOLDIERS_INCREMENT, number_of_castles);
            SDL_RenderCopy(mainRenderer, main_background_texture, NULL, NULL);
            render_map(castles, number_of_castles, mainRenderer);
            SDL_RenderPresent(mainRenderer);
            SDL_Delay(1000/FPS);
        }
    }

    SDL_DestroyTexture(main_background_texture);
    SDL_DestroyTexture(menu_texture);
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainRenderer);

    // shut everything down
    SDL_CloseAudio(); // stop plating the background music
    SDL_FreeWAV(wav_buffer);
    SDL_Quit();
    return 0;
}