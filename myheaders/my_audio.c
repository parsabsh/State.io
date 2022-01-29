#include "my_audio.h"
#include <stdio.h>

void my_audio_callback(void *userdata, Uint8 *stream, int len) {
    if (audio_len == 0) return;
    len = (len > audio_len ? (int)audio_len : len);
    SDL_memcpy (stream, audio_pos, len); // simply copy from one buffer into the other
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME); // mix from one buffer into another
    audio_pos += len;
    audio_len -= len;
}

void load_music(char path[]){
    // Load the music in wave format
    if(SDL_LoadWAV(path, &wav_spec, &wav_buffer, &wav_length) == NULL){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Audio loading failed!",SDL_GetError(),NULL);
    }
    wav_spec.callback = my_audio_callback;
    wav_spec.userdata = NULL;
    audio_pos = wav_buffer; // copy sound buffer
    audio_len = wav_length; // copy file length

    // Open the audio device
    if (SDL_OpenAudio(&wav_spec, NULL) < 0){
        printf("Couldn't open audio: %s\n", SDL_GetError());
    }
}

void check_music_finished(){
    if(audio_len == 0){
        audio_len = wav_length;
        audio_pos = wav_buffer;
    }
}