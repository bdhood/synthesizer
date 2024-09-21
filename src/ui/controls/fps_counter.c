
#include "SDL.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fps_counter.h"

FPS_COUNTER *fps_counter_init() {
    FPS_COUNTER *fps_counter = malloc(sizeof(FPS_COUNTER));
    fps_counter->nFrames = 0;
    fps_counter->timeSinceUpdate = 0.0f;
    fps_counter->fpsBuffer = malloc(32);
    memset(fps_counter->fpsBuffer, 0, 32);
    fps_counter->label = label_init();
    fps_counter->label->text = fps_counter->fpsBuffer;
    return fps_counter;
}

void fps_counter_free(FPS_COUNTER *fps_counter) {
    free(fps_counter);
}

void fps_counter_render(FPS_COUNTER *fps_counter, SDL_Renderer *r) {
    label_render(fps_counter->label, r);
    fps_counter->nFrames++;
}

void fps_counter_update(FPS_COUNTER *fps_counter, double deltaTime) {
    fps_counter->timeSinceUpdate += deltaTime;
    if (fps_counter->timeSinceUpdate >= 1.0f) {
        fps_counter->timeSinceUpdate = 0.0f;
        sprintf(fps_counter->fpsBuffer, "%d", (int)fps_counter->nFrames);
        fps_counter->nFrames = 0;
    }
}