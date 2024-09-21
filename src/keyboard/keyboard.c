#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL.h"

#include "note.h"
#include "keymap.h"
#include "freqmap.h"
#include "activekeys.h"
#include "synth.h"
#include "mixer.h"
#include "activenotes.h"

void keyboard_start() {
	keymap_load();
	freqmap_load();
	activekeys_load();
	activenotes_load();
}

void keyboard_event(SDL_Event* e) {
	if (e->type != SDL_KEYDOWN && e->type != SDL_KEYUP) {
		return;
	}
	NOTE *n;
	int key = e->key.keysym.sym;
	if (key == SDLK_LSHIFT) {
		key = 0xa0;
	}
	else if (key == SDLK_INSERT) {
		key = 0x2d;
	}
	else if (key == SDLK_END) {
		key = 0xf0;
	}
	else if (key == SDLK_PAGEUP) {
		key = 0x21;
	}
	else if (key == SDLK_PAGEDOWN) {
		key = 0x22;
	}
    switch (e->type) {
    case SDL_KEYDOWN:
		n = keymap_get(key);
		if (n == 0) {
			break;
		}
        activekeys_add(key);
		activenotes_add(n);
        break;
    case SDL_KEYUP:
		n = keymap_get(key);
		if (n == 0) {
			break;
		}
        activekeys_rm(key);
		activenotes_rm(n);
        break;
	}
}
