#include "SDL.h"
#include "ui.h"
#include "fileio.h"

#include "fps_counter.h"
#include "waveform.h"

#include "keyboard.h"

#include "window.h"
#include <stdbool.h>
#include <stdio.h>


#define SCREEN_WIDTH 990
#define SCREEN_HEIGHT 556

UI* ui = NULL;

void ui_event(SDL_Event* e) {
    //keyboard_event(e);
}

void ui_render() {
    fps_counter_render(ui->fps_counter, ui->window->renderer);
    waveform_render(ui->wf, ui->window->renderer);
    keyboard_ctrl_render(ui->kb, ui->window->renderer);
    notes_text_ctrl_render(ui->ntc, ui->window->renderer);
}

void ui_update(double deltaTime) {
    fps_counter_update(ui->fps_counter, deltaTime);
    notes_text_ctrl_update(ui->ntc, deltaTime);
}

void ui_start(SETTINGS *settings)
{
    if (ui != NULL) {
        return;
    }
    ui = (UI*)malloc(sizeof(UI));

    if (ui == 0) {
        printf("ERROR: ui_start() malloc returned 0\n");
        return;
    }

    ui->window = window_init("Synthesizer", SCREEN_WIDTH, SCREEN_HEIGHT);
    ui->window->target_fps = (float)settings->target_fps;

    TTF_Font* f1 = TTF_OpenFont("fonts/Segoe UI-Light.ttf", 14);

    ui->fps_counter = fps_counter_init();
    ui->fps_counter->label->font = f1;
    ui->fps_counter->label->color = (SDL_Color){ 150, 205, 50, 0 };
    ui->fps_counter->label->center = false;
    ui->fps_counter->label->rect = (SDL_Rect){ SCREEN_WIDTH - 80, SCREEN_HEIGHT - 30, 0, 0 };

    ui->wf = waveform_init();
    ui->wf->rect = (SDL_Rect){ 10, 10, SCREEN_WIDTH - 20, 300 };
    ui->wf->color_bg = (SDL_Color){ 0, 0, 0, 0 };
    ui->wf->color_left = (SDL_Color){ 200, 200, 0, 0 };
    ui->wf->color_right = (SDL_Color){ 0, 0, 200, 0 };

    ui->kb = keyboard_ctrl_init(settings->keys_root, (SDL_Rect) { 10, 310, 700, 250 });
    ui->kb->label->font = f1;
    ui->kb->label->color = (SDL_Color){ 150, 205, 50, 0 };
    ui->kb->label->center = true;
    ui->kb->BACKCOLOR = (SDL_Color){ 0, 0, 0, 0 };

    ui->kb->UNUSED_TRIM = (SDL_Color){ 130, 130, 130, 0 };
    ui->kb->UNUSED_TOP = (SDL_Color){ 125, 125, 125, 0 };
    ui->kb->UNUSED_BOTTOM = (SDL_Color){ 125, 125, 125, 0 };

    ui->kb->ACTIVE_TRIM = (SDL_Color){ 200, 200, 200, 0 };
    ui->kb->ACTIVE_TOP = (SDL_Color){ 74, 106, 26, 0 };
    ui->kb->ACTIVE_BOTTOM = (SDL_Color){ 65, 85, 15, 0 };

    ui->kb->INACTIVE_TRIM = (SDL_Color){ 100, 100, 100, 0 };
    ui->kb->INACTIVE_TOP = (SDL_Color){ 30, 30, 30, 0 };
    ui->kb->INACTIVE_BOTTOM = (SDL_Color){ 15, 15, 15, 0 };

    ui->ntc = notes_text_ctrl_init();
    ui->ntc->label->rect = (SDL_Rect){ 10, SCREEN_HEIGHT - 25, 0, 0 };
    ui->ntc->label->font = f1;
    ui->ntc->label->color = (SDL_Color){ 150, 205, 50, 0 };
    ui->ntc->label->center = false;

    window_mainloop(ui->window, ui_event, ui_render, ui_update);
}