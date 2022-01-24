#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
//#ifdef main
//#undef main
//#endif
// screen dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS 60

static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

// prototype for audio callback
void my_audio_callback(void *userdata, Uint8 *stream, int len);

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("SDL initialization failed! SDL_ERROR : %s\n", SDL_GetError());
    }

    // create a window
    SDL_Window *getUserNameWindow = SDL_CreateWindow("Login page", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                             SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    // create a renderer
    SDL_Renderer *getUserNameRenderer = SDL_CreateRenderer(getUserNameWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(getUserNameRenderer, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    // local variables to set background music
    static Uint32 wav_length;
    static Uint8 *wav_buffer; // buffer containing our audio file
    static SDL_AudioSpec wav_spec; // the specs of our piece of music

    // Load the WAV
    if(SDL_LoadWAV("music/pirates_of_caribbean.wav", &wav_spec, &wav_buffer, &wav_length) == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Missing file",
                                 SDL_GetError(),
                                 NULL);
        return 1;
    }
    // set the callback function
    wav_spec.callback = my_audio_callback;
    wav_spec.userdata = NULL;
    // set our global static variables
    audio_pos = wav_buffer; // copy sound buffer
    audio_len = wav_length; // copy file length

    // Open the audio device
    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
        printf("Couldn't open audio: %s\n", SDL_GetError());
        exit(-1);
    }
    // Start playing
    SDL_PauseAudio(0);

    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE){
        SDL_RenderClear(getUserNameRenderer);

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            if (sdlEvent.type == SDL_QUIT) {
                shallExit = SDL_TRUE;
                break;
            }
        }
        SDL_RenderPresent(getUserNameRenderer);
        SDL_Delay(1000 / FPS);
    }
    SDL_DestroyWindow(getUserNameWindow);
    // shut everything down
    SDL_CloseAudio();
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