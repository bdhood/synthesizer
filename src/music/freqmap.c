#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "note.h"

#define OCTAVES 8

#define FREQ_ADD(A, B, C, D) freqmap[A][B][C] = (float)(a4 * powl(twelth_root_of_2, D));

float freqmap[11][2][OCTAVES];

float freqmap_get(NOTE *n) {
	if (n->sharp >= 2 || n->octave > OCTAVES || n->sharp < 0 || n->octave < 0) {
		printf("invalid note");
		exit(1);
	}
	return freqmap[note_to_int(n) % 12][n->sharp][n->octave];
}

void freqmap_load() {
	memset(freqmap, 0, sizeof(freqmap));
	
	double twelth_root_of_2 = powl(2.0, (1.0 / 12.0));
	double a4 = 440.0;
	NOTE n;
	for (int octave = 0; octave < OCTAVES; octave++) {
		for (int note = 0; note < 12; note++) {
			note_from_int(&n, (octave * 12 + note));
			freqmap[note][n.sharp][octave] = (float)(a4 * powl(twelth_root_of_2, (octave * 12 + note) - 57));
		}
	}
}