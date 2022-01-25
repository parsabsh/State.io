#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
#ifdef main
#undef main
#endif
// screen dimensions
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 640
#define FPS 60

static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

// prototype for audio callback
void my_audio_callback(void *userdata, Uint8 *stream, int len);

int main(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("SDL initialization failed! SDL_ERROR : %s\n", SDL_GetError());
    }

    // create a window
    SDL_Window *getUserNameWindow = SDL_CreateWindow("Login page", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *getUserNameRenderer = SDL_CreateRenderer(getUserNameWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
//    SDL_SetRenderDrawColor(getUserNameRenderer, 0x64, 0x06, 0xad, SDL_ALPHA_OPAQUE);

    // local variables to set background music
    static Uint32 wav_length;
    static Uint8 *wav_buffer; // buffer containing our audio file
    static SDL_AudioSpec wav_spec; // the specs of our piece of music

    // Load the music in wave format
    if(SDL_LoadWAV("music/pirates_of_caribbean.wav", &wav_spec, &wav_buffer, &wav_length) == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Audio loading failed!",SDL_GetError(),getUserNameWindow);
        return 1;
    }
    wav_spec.callback = my_audio_callback;
    wav_spec.userdata = NULL;
    audio_pos = wav_buffer; // copy sound buffer
    audio_len = wav_length; // copy file length

    // Open the audio device
    if (SDL_OpenAudio(&wav_spec, NULL) < 0){
        printf("Couldn't open audio: %s\n", SDL_GetError());
        return 1;
    }
    // Start playing the background music
    SDL_PauseAudio(0);

    // load menu_login background image
    SDL_Surface * menu_login_image = SDL_LoadBMP("images/menu_login.bmp");
    SDL_Texture * menu_login_texture = SDL_CreateTextureFromSurface(getUserNameRenderer, menu_login_image);
    SDL_FreeSurface(menu_login_image);

    // the login window
    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE){
        SDL_RenderClear(getUserNameRenderer);

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)){
            if (sdlEvent.type == SDL_QUIT){
                shallExit = SDL_TRUE;
                break;
            }
        }
        SDL_RenderCopy(getUserNameRenderer, menu_login_texture, NULL, NULL);
        SDL_RenderPresent(getUserNameRenderer);
        SDL_Delay(1000 / FPS);
    }

    // close the login window
    SDL_DestroyTexture(menu_login_texture);
    SDL_DestroyWindow(getUserNameWindow);
    SDL_DestroyRenderer(getUserNameRenderer);

    // shut everything down
    SDL_CloseAudio(); // stop plating the background music
    SDL_FreeWAV(wav_buffer);
    SDL_Quit();
    return 0;
}

void my_audio_callback(void *userdata, Uint8 *stream, int len) {
    if (audio_len == 0) return;
    len = (len > audio_len ? (int)audio_len : len);
    SDL_memcpy (stream, audio_pos, len); // simply copy from one buffer into the other
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME); // mix from one buffer into another
    audio_pos += len;
    audio_len -= len;
}