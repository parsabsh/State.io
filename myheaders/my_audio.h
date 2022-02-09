/*
    I learned how to work with sdl_audio from this repository :
    https://gist.github.com/armornick/3447121
*/

#include <SDL2/SDL.h>

// declaration
extern Uint8 *audio_pos; // global pointer to the audio buffer to be played
extern Uint32 audio_len; // remaining length of the sample we have to play
extern Uint32 wav_length;
extern Uint8 *wav_buffer; // buffer containing our audio file
extern SDL_AudioSpec wav_spec; // the specs of our piece of music

// audio callback function
void my_audio_callback(void *userdata, Uint8 *stream, int len);

// load a .wav music file
void load_music(char path[]);

// if music is finished, play it again
void check_music_finished();