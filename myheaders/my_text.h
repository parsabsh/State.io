#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font* comic_font;
extern TTF_Font* consola_font;

void load_fonts();

void print_text(TTF_Font* font, char* text, SDL_Color color, SDL_Renderer* renderer, SDL_Rect* rect);

// returns 0 when "ENTER" key is pressed
int text_input(SDL_Event event, char* user_name, SDL_Rect* rect, SDL_Window* window, int max_size);