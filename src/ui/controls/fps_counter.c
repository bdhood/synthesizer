
#include "SDL.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fps_counter.h"

FPS_COUNTER *fps_counter_init() {
    FPS_COUNTER *fps_counter = malloc(sizeof(FPS_COUNTER));
    fps_counter->nFrames = 0;
    fps_counter->nFramesPrev = 0;
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
        //itoa((int)fps_counter->nFrames + fps_counter->nFramesPrev, fps_counter->fpsBuffer, 10);
        //fps_counter->nFramesPrev = fps_counter->nFrames;
        fps_counter->nFrames = 0;
    }
    /*
    fps_counter->avgFps += (double)fps_counter->nFrames / deltaTime;
    fps_counter->nFpsInAvg++;
    fps_counter->timeSinceUpdate += deltaTime;
    fps_counter->nFrames = 0;

    if (fps_counter->timeSinceUpdate >= 1.0f) {
        fps_counter->timeSinceUpdate = 0.0f;

        double fps = fps_counter->avgFps / (double)fps_counter->nFpsInAvg;
        fps_counter->avgFps = 0.0f;
        fps_counter->nFpsInAvg = 0;
        itoa((int)fps, fps_counter->fpsBuffer, 10);
    }
    */

    /*
    Uint32 currentTicks = SDL_GetTicks();
    fps_counter->nFrames++;

    fps_counter->deltaTime = (currentTicks - fps_counter->lastTicks) / 1000.f;

    if (fps_counter->deltaTime > 0.0f) {
        double fps = fps_counter->nFrames / fps_counter->deltaTime;
        fps_counter->avgFps += fps;
        fps_counter->nFpsInAvg++;
        fps_counter->nFrames = 0;
        fps_counter->lastTicks = currentTicks;
        if (currentTicks - fps_counter->lastFpsUpdate >= 250) {
            fps_counter->lastFpsUpdate = currentTicks;
            fps = fps_counter->avgFps / (double)fps_counter->nFpsInAvg;
            fps_counter->avgFps = 0.f;
            fps_counter->nFpsInAvg = 0;
            itoa((int)fps, fps_counter->fpsBuffer, 10);
        }
    }
    */
}