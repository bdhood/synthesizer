#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "SDL.h"
#include "cJSON.h"
#include "note.h"
#include "transpose.h"
#include "activekeys.h"
#include "activenotes.h"
#include "fileio.h"

#define ASSIGN(A, B, C, D) 	keymap[A].note = B; \
							keymap[A].sharp = C; \
							keymap[A].octave = D;
					
int transpose = 0;

#define N_NOTES 0x100
NOTE keymap[N_NOTES];

int keymap_get_transpose() {
	return transpose;
}

void keymap_add_transpose(int x) {
	for (int i = 0; i < N_NOTES; i++) {
		if (keymap[i].note != 0)
			transpose_add(&keymap[i], -transpose);
	}
	transpose = x;
	for (int i = 0; i < N_NOTES; i++) {
		if (keymap[i].note != 0)
			transpose_add(&keymap[i], transpose);
	}
}

void keymap_load(cJSON *keys_root) {
	memset(keymap, 0, N_NOTES * sizeof(NOTE));
	char* note_str;
	int note_loc;
	for (cJSON *i = keys_root->child; i != 0; i = i->next) {

		note_loc = -1;
		note_str = 0;
		for (cJSON* j = i->child; j != 0; j = j->next) {
			if (strcmp(j->string, "keyCode") == 0) {
				if (j->valuestring != 0) {
					note_loc = j->valuestring[0];
				}
				else {
					note_loc = j->valueint;
				}
			}
			if (strcmp(j->string, "note") == 0) {
				note_str = j->valuestring;
			}
		}

		if (note_loc == -1 || note_str == 0) {
			continue;
		}

		note_from_string(&keymap[note_loc], note_str);
	}
}

NOTE *keymap_get(int c) {
	if (c >= 0 && c < N_NOTES) {
		return &keymap[c];
	}
	return 0;
}