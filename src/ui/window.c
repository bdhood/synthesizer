#include <stdlib.h>
#include <stdio.h>

#include "window.h"
#include "keyboard.h"
#include "synth.h"

#include "SDL_ttf.h"

WINDOW *window_init(char *title, int width, int height) {
    
    TTF_Init();
    
    WINDOW *w = malloc(sizeof(WINDOW));

    w->abort = false;
    w->break_mainloop = false;
    w->lastTick = SDL_GetTicks();
    
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		printf("Could not initialize SDL2: %s\n", SDL_GetError());
		exit(1);
	}

	w->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (w->window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		exit(1);
	}

    w->renderer = SDL_CreateRenderer(w->window, -1, 0);
    return w;
}

void window_mainloop(WINDOW *window, void (*event)(SDL_Event*), void (*render)(), void (*update)(double)) {
    double remainder = 0.0f;
    double targetHz = 1.0f / window->target_fps;
    while (true) {
        for (int i = 0; SDL_PollEvent(&window->event) != 0; i++) {
            if (window->event.type == SDL_QUIT) {
                SDL_Quit();
                synth_off();
                exit(0);
            }
            event(&window->event);
            if (i >= 4) {
                break;
            }
        }
        
        if (window->abort) {
            SDL_Quit();
            synth_off();
            exit(0);
        }

        if (window->break_mainloop) {
            window->break_mainloop = false;
            return;
        }

        if (SDL_RenderClear(window->renderer) == -1) {
            continue;
        }

        Uint32 currentTicks = SDL_GetTicks();
        double deltaTime = ((double)currentTicks - (double)window->lastTick) / 1000.f;

        if (deltaTime + remainder > targetHz) {
            window->clock += deltaTime;
            update(deltaTime);
            window->lastTick = currentTicks;
            remainder += deltaTime - targetHz;
            if (remainder > targetHz) {
                remainder = targetHz;
            }
            render();
            SDL_RenderPresent(window->renderer);
        }
        else {
            SDL_Delay(1);
        }
    }
}