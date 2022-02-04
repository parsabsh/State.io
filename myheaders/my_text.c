#include "my_text.h"

TTF_Font* comic_font;
TTF_Font* consola_font;

void load_fonts(){
    comic_font = TTF_OpenFont("fonts/comic.ttf", 32);
    consola_font = TTF_OpenFont("fonts/consola.ttf", 32);
}

void print_text(TTF_Font* font, char* text, SDL_Color color, SDL_Renderer* renderer, SDL_Rect* rect){
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
}

int text_input(SDL_Event event, char* user_name, SDL_Rect* rect, SDL_Window* window, int max_size){
    // handle text input
    if(event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN){
        // handle backspace
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && strlen(user_name) > 0) {
            user_name[strlen(user_name)-1] = '\0';
            (*rect).x += 10;
            (*rect).w -= 20;
        }
        else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){
            if(strlen(user_name) > 0){
                return 0;
            }else{
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Empty Box!", "you should enter a valid user_name", NULL);
            }
        }
        else if (event.type == SDL_TEXTINPUT){
            strcat(user_name, event.text.text);
            if(strlen(user_name) > max_size){
                user_name[strlen(user_name)-1] = '\0';
            }else {
                (*rect).x -= 10;
                (*rect).w += 20;
            }
        }
    }
    return 1;
}