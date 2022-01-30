#include "my_image.h"

SDL_Texture* load_bmp_image(char* path, SDL_Renderer* renderer){
    SDL_Surface* surface = SDL_LoadBMP(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // free the surface because we don't need it anymore
    return texture;
}