#pragma once

#include "SDL.h"

#include "label.h"

typedef struct NOTES_TEXT_CTRL {
    LABEL *label;
    char *buffer;
} NOTES_TEXT_CTRL;

NOTES_TEXT_CTRL *notes_text_ctrl_init();
void notes_text_ctrl_free(NOTES_TEXT_CTRL *ntc);
void notes_text_ctrl_render(NOTES_TEXT_CTRL *ntc, SDL_Renderer *r);
void notes_text_ctrl_update(NOTES_TEXT_CTRL *ntc, double deltaTime);