#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "note.h"
#include "transpose.h"

#define ASSIGN(A, B, C, D) 	keymap[A].note = B; \
							keymap[A].sharp = C; \
							keymap[A].octave = D;
					
int transpose = 0;

NOTE keymap[0x100];

int keymap_get_transpose() {
	return transpose;
}

void keymap_set_transpose(int x) {
	for (int i = 0; i < 0x100; i++) {
		if (keymap[i].note != 0)
			transpose_add(&keymap[i], -transpose);
	}
	transpose = x;
	for (int i = 0; i < 0x100; i++) {
		if (keymap[i].note != 0)
			transpose_add(&keymap[i], transpose);
	}
}

void keymap_load() {
	memset(keymap, 0, 0x100 * sizeof(NOTE));
	// Lower Keyboard
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
	
	// Upper Keyboard
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
}

NOTE *keymap_get(unsigned char c) {
	return &keymap[c];
}