#ifndef UI_H
#define UI_H

#include "SDL.h"
#include "window.h"
#include "settings.h"

#include "fps_counter.h"
#include "waveform.h"
#include "keyboard_ctrl.h"
#include "notes_text_ctrl.h"

typedef struct UI {
    WINDOW* window;
    FPS_COUNTER* fps_counter;
    WAVEFORM* wf;
    KEYBOARD_CTRL* kb;
    NOTES_TEXT_CTRL* ntc;
} UI;


void ui_start(SETTINGS *settings);

#endif