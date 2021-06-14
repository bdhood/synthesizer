
#include "SDL.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "notes_text_ctrl.h"
#include "note.h"
#include "activenotes.h"

NOTES_TEXT_CTRL *notes_text_ctrl_init() {
    NOTES_TEXT_CTRL *ntc = malloc(sizeof(NOTES_TEXT_CTRL));
    if (ntc == 0) {
        return 0;
    }
    ntc->label = label_init();
    ntc->buffer = malloc(256);
    if (ntc->buffer == 0) {
        label_free(ntc->label);
        free(ntc);
        return 0;
    }
    memset(ntc->buffer, 0, 256);
    ntc->label->text = ntc->buffer;
    return ntc;
}
void notes_text_ctrl_free(NOTES_TEXT_CTRL *ntc) {
    label_free(ntc->label);
    free(ntc->buffer);
    free(ntc);
}
void notes_text_ctrl_render(NOTES_TEXT_CTRL *ntc, SDL_Renderer *r) {
    label_render(ntc->label, r);
}

void notes_text_ctrl_update(NOTES_TEXT_CTRL *ntc, double deltaTime) {
    NOTE nStart;
    nStart.note = 'a';
    nStart.octave = 0;
    nStart.sharp = 0;
    memset(ntc->buffer, 0, 256);
    NOTE* n = activenotes_next(&nStart);
    while (n != 0) {
        ntc->buffer[strlen(ntc->buffer)] = n->note;
        if (n->sharp) {
            ntc->buffer[strlen(ntc->buffer)] = '#';
        }
        ntc->buffer[strlen(ntc->buffer)] = n->octave + '0';
        if (!n->sharp) {
            ntc->buffer[strlen(ntc->buffer)] = ' ';
        }
        ntc->buffer[strlen(ntc->buffer)] = ' ';
        n = activenotes_next(n);
    }
}