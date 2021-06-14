#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "SDL.h"

typedef struct WAVEFORM {
    SDL_Rect rect;
    SDL_Color color_bg;
    SDL_Color color_left;
    SDL_Color color_right;
} WAVEFORM;

WAVEFORM* waveform_init();
void waveform_free(WAVEFORM* wf);
void waveform_render(WAVEFORM* wf, SDL_Renderer* r);

#endif