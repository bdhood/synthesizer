#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>

typedef struct LABEL {
    char *text;
    SDL_Rect rect;
    TTF_Font *font;
    SDL_Color color;
    bool center;
} LABEL;

LABEL *label_init();
void label_free(LABEL *label);
void label_render(LABEL *label, SDL_Renderer *r);
