#pragma once

#include <stdbool.h>

#include "SDL.h"
//#include "SDL_ttf.h"

typedef struct WINDOW {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    Uint32 lastTick;
    bool abort;
    bool break_mainloop;
    float target_fps;
} WINDOW;

WINDOW *window_init(char *title, int width, int height);
void window_mainloop(WINDOW *window, void (*event)(SDL_Event*), void (*render)(), void (*update)(double));