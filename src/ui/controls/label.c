#include <stdlib.h>
#include <stdbool.h>
#include "label.h"

LABEL *label_init() {
    LABEL *label = malloc(sizeof(LABEL));
    if (label == 0) {
        return 0;
    }
    label->center = false;
    return label;
}

void label_free(LABEL *label) {
    free(label);
}

void label_render(LABEL *label, SDL_Renderer *r) {
    SDL_Surface *s = TTF_RenderText_Blended(label->font, label->text, label->color);

    if (s == NULL) {
        return;
    }
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s); 
    if (t == NULL) {
        SDL_FreeSurface(s);
        return;
    }
    
    SDL_Rect rect;
    rect.x = label->rect.x;
    rect.y = label->rect.y;
    rect.w = s->w;
    rect.h = s->h;
    if (label->center) {
        rect.x += (label->rect.w - s->w) / 2;
        rect.y += (label->rect.h - s->h) / 2;
    }

    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = s->w;
    src_rect.h = s->h;

    SDL_RenderCopy(r, t, &src_rect, &rect);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);

}