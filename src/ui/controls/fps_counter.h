#pragma once

#include "SDL.h"

#include "label.h"

typedef struct FPS_COUNTER {
    LABEL *label;
    double timeSinceUpdate;
    int nFrames;
    int nFramesPrev;
    char *fpsBuffer;
} FPS_COUNTER;

FPS_COUNTER *fps_counter_init();
void fps_counter_free(FPS_COUNTER *fps_counter);
void fps_counter_render(FPS_COUNTER *fps_counter, SDL_Renderer *r);
void fps_counter_update(FPS_COUNTER *fps_counter, double deltaTime);