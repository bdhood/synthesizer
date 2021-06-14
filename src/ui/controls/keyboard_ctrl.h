#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "cJSON.h"

#include "label.h"

typedef struct KEYBOARD_CTRL_KEY {
    SDL_Rect rect;
    char* text;
    unsigned char keyCode;
} KEYBOARD_CTRL_KEY;

typedef struct KEYBOARD_CTRL {
    SDL_Rect rect;
    LABEL* label;
    KEYBOARD_CTRL_KEY* keys;

    SDL_Color BACKCOLOR;
    SDL_Color UNUSED_TRIM;
    SDL_Color UNUSED_TOP;
    SDL_Color UNUSED_BOTTOM;

    SDL_Color ACTIVE_TRIM;
    SDL_Color ACTIVE_TOP;
    SDL_Color ACTIVE_BOTTOM;

    SDL_Color INACTIVE_TRIM;
    SDL_Color INACTIVE_TOP;
    SDL_Color INACTIVE_BOTTOM;
} KEYBOARD_CTRL;

KEYBOARD_CTRL* keyboard_ctrl_init(cJSON* keys_root, SDL_Rect rect);
void keyboard_ctrl_free(KEYBOARD_CTRL* keyboard_ctrl);
void keyboard_ctrl_render(KEYBOARD_CTRL* keyboard_ctrl, SDL_Renderer* r);
