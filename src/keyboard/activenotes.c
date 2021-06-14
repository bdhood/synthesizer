#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "note.h"

#define SIZE 0x100

char active_notes[SIZE][2][7];

void activenotes_load() {
	memset(active_notes, 0, sizeof(active_notes));
}

void activenotes_add(NOTE *n) {
	active_notes[n->note][n->sharp][n->octave] = 1;
}

void activenotes_rm(NOTE *n) {
	active_notes[n->note][n->sharp][n->octave] = 0;
}

bool activenotes_isactive(NOTE *n) {
	return active_notes[n->note][n->sharp][n->octave] == 1;
}

NOTE *activenotes_next(NOTE *n) {


	while (1) {
		if (n->sharp == 0) {
			n->sharp = 1;
		}
		else {
			n->sharp = 0;
			n->note++;
		}

		if (n->note > 'g') {
			n->note = 'a';
			n->octave++;
		}

		if (n->octave > 8) {
			return 0;
		}

		if (activenotes_isactive(n))
			return n;
	}
	return 0;
}

int activenotes_get_total() {
	int total = 0;
	NOTE n;
	n.note = 'a';
	n.sharp = 0;
	n.octave = 0;
	while (n.octave < 8) {
		if (n.sharp == 0) {
			n.sharp = 1;
		}
		else {
			n.sharp = 0;
			n.note++;
		}

		if (n.note > 'g') {
			n.note = 'a';
			n.octave++;
		}
		if (activenotes_isactive(&n)) {
			total++;
		}
	}
	return total;
}
