#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "note.h"

char active[0x100][2][7];

void activenotes_load() {
	memset(active, 0, sizeof(active));
}

void activenotes_add(NOTE *n) {
	active[n->note][n->sharp][n->octave] = 1;
}

void activenotes_rm(NOTE *n) {
	active[n->note][n->sharp][n->octave] = 0;
}

bool activenotes_isactive(NOTE *n) {
	return active[n->note][n->sharp][n->octave] == 1;
}