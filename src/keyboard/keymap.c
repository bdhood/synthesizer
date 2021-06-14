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

void keymap_set_transpose(int x) {
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

	/* sdl2 key mapping
	// Lower Keyboard
	ASSIGN(0xa0, 'a', 0, 2); // left shift
	ASSIGN('z', 'c', 0, 3);
	ASSIGN('s', 'c', 1, 3);
	ASSIGN('x', 'd', 0, 3);
	ASSIGN('d', 'd', 1, 3);
	ASSIGN('c', 'e', 0, 3);
	ASSIGN('v', 'f', 0, 3);
	ASSIGN('g', 'f', 1, 3);
	ASSIGN('b', 'g', 0, 3);
	ASSIGN('h', 'g', 1, 3);
	ASSIGN('n', 'a', 0, 3);
	ASSIGN('j', 'a', 1, 3);
	ASSIGN('m', 'b', 0, 3);
	ASSIGN(0x2c, 'c', 0, 4); // <
	ASSIGN('l', 'c', 1, 4);
	ASSIGN(0x2e, 'd', 0, 4); // >
	ASSIGN(0x3b, 'd', 1, 4); // ;
	ASSIGN(0x2f, 'e', 0, 4); // /

	// Upper Keyboard - lower octave
	ASSIGN('q', 'c', 0, 4);
	ASSIGN('2', 'c', 1, 4);
	ASSIGN('w', 'd', 0, 4);
	ASSIGN('3', 'd', 1, 4);
	ASSIGN('e', 'e', 0, 4);
	ASSIGN('r', 'f', 0, 4);
	ASSIGN('5', 'f', 1, 4);
	ASSIGN('t', 'g', 0, 4);
	ASSIGN('6', 'g', 1, 4);
	ASSIGN('y', 'a', 0, 4);
	ASSIGN('7', 'a', 1, 4);
	ASSIGN('u', 'b', 0, 4);
	ASSIGN('i', 'c', 0, 5); 
	ASSIGN('9', 'c', 1, 5);
	ASSIGN('o', 'd', 0, 5);
	ASSIGN('0', 'd', 1, 5);
	ASSIGN('p', 'e', 0, 5); 
	ASSIGN(0x5b, 'f', 0, 5); // [
	ASSIGN(0x3d, 'f', 1, 5); // =
	ASSIGN(0x5d, 'g', 0, 5); // ]
	ASSIGN(0x08, 'a', 1, 5); // [Bksp]
	ASSIGN(0x5c, 'a', 0, 5); // backslash
	ASSIGN(0x2d, 'b', 0, 5);   // insert
	ASSIGN(0x7f, 'c', 0, 6);   // delete
	ASSIGN(0xf0, 'd', 0, 6);	 // end
	ASSIGN(0x21, 'e', 0, 6);   // pageup
	ASSIGN(0x22, 'f', 0, 6);   // pagedown
	*/

	/* windows
	ASSIGN(0xa0, 'a', 0, 2); // left shift
	ASSIGN('Z', 'c', 0, 3);
	ASSIGN('S', 'c', 1, 3);
	ASSIGN('X', 'd', 0, 3);
	ASSIGN('D', 'd', 1, 3);
	ASSIGN('C', 'e', 0, 3);
	ASSIGN('V', 'f', 0, 3);
	ASSIGN('G', 'f', 1, 3);
	ASSIGN('B', 'g', 0, 3);
	ASSIGN('H', 'g', 1, 3);
	ASSIGN('N', 'a', 0, 3);
	ASSIGN('J', 'a', 1, 3);
	ASSIGN('M', 'b', 0, 3);
	ASSIGN(0xbc, 'c', 0, 4); // <
	ASSIGN('L', 'c', 1, 4);
	ASSIGN(0xbe, 'd', 0, 4); // >
	ASSIGN(0xba, 'd', 1, 4); // ;
	ASSIGN(0xbf, 'e', 0, 4); // /

	// Upper Keyboard - lower octave
	ASSIGN('Q', 'c', 0, 4);
	ASSIGN('2', 'c', 1, 4);
	ASSIGN('W', 'd', 0, 4);
	ASSIGN('3', 'd', 1, 4);
	ASSIGN('E', 'e', 0, 4);
	ASSIGN('R', 'f', 0, 4);
	ASSIGN('5', 'f', 1, 4);
	ASSIGN('T', 'g', 0, 4);
	ASSIGN('6', 'g', 1, 4);
	ASSIGN('Y', 'a', 0, 4);
	ASSIGN('7', 'a', 1, 4);
	ASSIGN('U', 'b', 0, 4);
	ASSIGN('I', 'c', 0, 5);
	ASSIGN('9', 'c', 1, 5);
	ASSIGN('O', 'd', 0, 5);
	ASSIGN('0', 'd', 1, 5);
	ASSIGN('P', 'e', 0, 5);
	ASSIGN(0xdb, 'f', 0, 5); // [
	ASSIGN(0xbb, 'f', 1, 5); // =
	ASSIGN(0xdd, 'g', 0, 5); // ]
	ASSIGN(0x08, 'a', 1, 5); // [Bksp]
	ASSIGN(0xdc, 'a', 0, 5); // backslash
	ASSIGN(45, 'b', 0, 5);   // insert
	ASSIGN(46, 'c', 0, 6);   // delete
	ASSIGN(35, 'd', 0, 6);	 // end
	ASSIGN(33, 'e', 0, 6);   // pageup
	ASSIGN(34, 'f', 0, 6);   // pagedown
	*/
}

NOTE *keymap_get(int c) {
	if (c < N_NOTES) {
		return &keymap[c];
	}
	return 0;
}